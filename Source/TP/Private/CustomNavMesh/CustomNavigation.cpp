// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomNavMesh/CustomNavigation.h"
#include "Kismet/GameplayStatics.h"
#include "CustomNavMesh/CustomNavUnit.h"

// Sets default values
ACustomNavigation::ACustomNavigation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACustomNavigation::Setup()
{
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomNavUnit::StaticClass(), Tiles);

	for (auto Tile : Tiles) {
		Units.Add(Cast<ACustomNavUnit>(Tile));
	}


	/*for (auto Unit : Units) {
		Unit->SetupNeightbors(Units);
	}*/

}

// Called when the game starts or when spawned
void ACustomNavigation::BeginPlay()
{
	//Super::BeginPlay();
}

ACustomNavUnit* ACustomNavigation::GetCurrentUnit(FVector CurrentLocation)
{
	ACustomNavUnit* closestUnit = nullptr;
	double closestDistance = 99999999.f;
	/*ACustomNavUnit* closestUnit = Units[0];
	double closestDistance = (CurrentLocation - Units[0]->GetActorLocation()).Size();*/

	for (auto Unit : Units) {
		double distance = (CurrentLocation - Unit->GetActorLocation()).Size();
		if (distance < closestDistance) {
			closestUnit = Unit;
			closestDistance = distance;
		}
	}
	return closestUnit;
}

// Called every frame
void ACustomNavigation::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

ACustomNavUnit* ACustomNavigation::GetNextTarget(AActor* EndTarget, FVector CurrentLocation)
{
	ACustomNavUnit* CurrentUnit = GetCurrentUnit(CurrentLocation);

	TArray<ACustomNavUnit*> Neighbors = GetUnitNeighbors(CurrentUnit);

	ACustomNavUnit* closestNeighbor = nullptr;
	double closestDistance = 9999999.f;

	for (ACustomNavUnit* Neighbor : Neighbors) {
		double distance = (Neighbor->GetActorLocation() - EndTarget->GetActorLocation()).Size();
		if (distance < closestDistance) {
			closestNeighbor = Neighbor;
			closestDistance = distance;
		}
	}

	return closestNeighbor;

	//return CurrentUnit->GetNextTarget(EndTarget);
	//return nullptr;
}

TArray<ACustomNavUnit*> ACustomNavigation::GetUnitNeighbors(ACustomNavUnit* TargetUnit)
{
	TArray<ACustomNavUnit*> Neighbors;
	for (auto Unit : Units) {
		if (Unit != TargetUnit) {
			float ManhattanDistance = abs(TargetUnit->GetActorLocation().X - Unit->GetActorLocation().X) + abs(TargetUnit->GetActorLocation().Y - Unit->GetActorLocation().Y);
			if (ManhattanDistance < 110.f) {
				Neighbors.AddUnique(Unit);
			}
		}
	}

	return Neighbors;
}

