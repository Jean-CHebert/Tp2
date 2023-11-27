// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TP/DebugMacros.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GameStatsActor.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "CustomNavMesh//CustomNavigation.h"
#include "Game/TpGameInstance.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	CustomNav = CreateDefaultSubobject<ACustomNavigation>(TEXT("CustomNav"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AAIController>(GetController());

	UTpGameInstance* GameInstance = Cast<UTpGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UsesNaiveNavigation = GameInstance->UsesNaiveMovement;
	GetCharacterMovement()->MaxWalkSpeed = GameInstance->EnemyMovementSpeed;

	if (PawnSensing) {
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	if (CustomNav) {
		CustomNav->Setup();
	}
	
	if (UsesNaiveNavigation) {
		NaiveNavigationTarget = CustomNav->GetNextTarget(PatrolTarget, GetActorLocation());
		MoveToTarget(NaiveNavigationTarget);
	}
	else {
		MoveToTarget(PatrolTarget);
	}
}

void AEnemy::Die()
{
	AGameStatsActor* GameStats = Cast<AGameStatsActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameStatsActor::StaticClass()));
	if (GameStats) {
		GameStats->EnemyDead();
	}

	Destroy();
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) {
		return false;
	}

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) {
		return;
	}
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets) {
		if (Target != PatrolTarget) {
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0) {
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (!UsesNaiveNavigation) {
		if (EnemyState == EEnemyState::EES_Chasing) {
			return;
		}

		if (SeenPawn->ActorHasTag(FName("TpCharacter"))) {
			if (EnemyState != EEnemyState::EES_Attacking) {
				EnemyState = EEnemyState::EES_Chasing;
			}
			GetWorldTimerManager().ClearTimer(PatrolTimer);
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			CombatTarget = SeenPawn;
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemy::PatrolTimerFinished()
{
	if (UsesNaiveNavigation) {
		MoveToTarget(NaiveNavigationTarget);
	}
	else {
		MoveToTarget(PatrolTarget);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UsesNaiveNavigation) {
		CheckNaiveNavigationTarget();
	}
	else {
		if (EnemyState > EEnemyState::EES_Patrolling) {
			CheckCombatTarget();
		}
		else {
			CheckPatrolTarget();
		}
	}
	
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius)) {
		EnemyState = EEnemyState::EES_Patrolling;
		MoveToTarget(PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing) {
		EnemyState = EEnemyState::EES_Chasing;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking) {
		EnemyState = EEnemyState::EES_Attacking;
	}
}



void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius)) {
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTimer = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTimer);
	}
}

void AEnemy::CheckNaiveNavigationTarget()
{
	if (InTargetRange(NaiveNavigationTarget, PatrolRadius)) {
		if (NaiveNavigationTarget == PatrolTarget) {
			PatrolTarget = ChoosePatrolTarget();
			NaiveNavigationTarget = CustomNav->GetNextTarget(PatrolTarget, GetActorLocation());
			const float WaitTimer = FMath::RandRange(WaitMin, WaitMax);
			GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTimer);
			return;
		}

		if ((GetActorLocation() - PatrolTarget->GetActorLocation()).Size() <= 150.f) {
			NaiveNavigationTarget = PatrolTarget;
		}
		else {
			NaiveNavigationTarget = CustomNav->GetNextTarget(PatrolTarget, GetActorLocation());
		}
		MoveToTarget(NaiveNavigationTarget);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint);


}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes) {
		Attributes->ReceiveDamage(DamageAmount);

		if (HealthBarWidget) {
			HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		}
		if (!Attributes->IsAlive()) {
			Die();
		}
	}

	if (!UsesNaiveNavigation) {
		CombatTarget = EventInstigator->GetPawn();
		EnemyState = EEnemyState::EES_Chasing;
		MoveToTarget(CombatTarget);
	}
	
	return DamageAmount;
}

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, Attributes);
	DOREPLIFETIME(AEnemy, HealthBarWidget);
}

