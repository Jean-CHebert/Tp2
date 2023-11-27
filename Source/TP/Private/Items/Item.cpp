// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "TP/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/TpCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	/*UE_LOG(LogTemp, Warning, TEXT("Hey"));
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString("Some message"));
	}*/
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATpCharacter* TpCharacter = Cast<ATpCharacter>(OtherActor);
	if (TpCharacter) {
		TpCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATpCharacter* TpCharacter = Cast<ATpCharacter>(OtherActor);
	if (TpCharacter) {
		TpCharacter->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FString Message = FString::Printf(TEXT("DeltaTime: %f"), DeltaTime);
	//GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);

	/*DRAW_SPHERE_SingleFrame(GetActorLocation());

	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);*/

	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime*100.f);

}

