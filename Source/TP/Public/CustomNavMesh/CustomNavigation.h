// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomNavigation.generated.h"

class ACustomNavUnit;

UCLASS()
class TP_API ACustomNavigation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomNavigation();

	void Setup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<ACustomNavUnit*> Units;
	ACustomNavUnit* GetCurrentUnit(FVector CurrentLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACustomNavUnit* GetNextTarget(AActor* EndTarget, FVector CurrentLocation);
	TArray<ACustomNavUnit*> GetUnitNeighbors(ACustomNavUnit* TargetUnit);

};
