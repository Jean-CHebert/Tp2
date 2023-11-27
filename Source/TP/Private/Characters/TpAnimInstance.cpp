// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TpAnimInstance.h"
#include "Characters/TpCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTpAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TpCharacter = Cast<ATpCharacter>(TryGetPawnOwner());
	if (TpCharacter) {
		TpCharacterMovement = TpCharacter->GetCharacterMovement();
	}
}

void UTpAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (TpCharacterMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(TpCharacterMovement->Velocity);
		IsFalling = TpCharacterMovement->IsFalling();
		CharacterState = TpCharacter->GetCharacterState();
	}
}
