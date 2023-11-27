// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TpGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TP_API UTpGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UsesNaiveMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyMovementSpeed = 150.f;
};
