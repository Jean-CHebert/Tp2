// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomNavUnit.generated.h"

UCLASS()
class TP_API ACustomNavUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomNavUnit();

	void SetupNeightbors(TArray<ACustomNavUnit*> AllUnits);
	ACustomNavUnit* GetNextTarget(AActor* EndTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

private:
	UPROPERTY()
	TArray<ACustomNavUnit*> Neighbors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
