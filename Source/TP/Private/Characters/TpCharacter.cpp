// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TpCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"

// Sets default values
ATpCharacter::ATpCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>("Hair");
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>("Eyebrows");
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void ATpCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("TpCharacter"));
}

void ATpCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		float MovementValue = ForwardAxisEmulator->UpdateValue(Value);

		AddMovementInput(Direction, MovementValue);
	}
}

void ATpCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		float MovementValue = SideWaysAxisEmulator->UpdateValue(Value);

		AddMovementInput(Direction, MovementValue);
	}
}

void ATpCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ATpCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ATpCharacter::ChargeKeyPressed()
{
	IsCharging = true;
}

void ATpCharacter::EKeyPressed_Implementation()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) {
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		EquippedWeapon = OverlappingWeapon;
	}
}

// Called every frame
void ATpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCharging) {
		ChargePercentage = FMath::Clamp(ChargePercentage + DeltaTime / 2, 0, 1);
	}

}

// Called to bind functionality to input
void ATpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATpCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATpCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATpCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATpCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &ATpCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Charge"), IE_Pressed, this, &ATpCharacter::ChargeKeyPressed);
}

void ATpCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ATpCharacter::OpenLobby()
{
	UWorld* World = GetWorld();
	if (World) {
		World->ServerTravel("/Game/Maps/MainMap?listen");
			
	}
}

void ATpCharacter::CallOpenLevel(const FString& Address)
{
	UGameplayStatics::OpenLevel(this, *Address);
}

void ATpCharacter::CallClientTravel(const FString& Address)
{
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController) {
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

