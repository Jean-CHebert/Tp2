// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TpGameInsance.generated.h"

/**
 * 
 */
UCLASS()
class TP_API UTpGameInsance : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	bool test;
};
