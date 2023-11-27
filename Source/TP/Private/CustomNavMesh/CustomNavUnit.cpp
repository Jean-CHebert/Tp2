// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomNavMesh/CustomNavUnit.h"

// Sets default values
ACustomNavUnit::ACustomNavUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}

void ACustomNavUnit::SetupNeightbors(TArray<ACustomNavUnit*> AllUnits)
{
	//FString Message = FString::Printf(TEXT("Units: %f"), GetActorLocation().Y);
	//FString Message2 = FString::Printf(TEXT("Units2: %i"), GetActorLocation().Y);
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, Message);
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, Message2);
	/*for (auto Unit : AllUnits) {
		if (Unit != this) {
			float ManhattanDistance = abs(this->GetActorLocation().X - Unit->GetActorLocation().X) + abs(this->GetActorLocation().Y - Unit->GetActorLocation().Y);
			//FString Messaget = FString::Printf(TEXT("Units: %f"), ManhattanDistance);
			//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, Messaget);
			if (ManhattanDistance < 1171.f) {
				Neighbors.AddUnique(Unit);
			}
		}
	}*/
	//FString Message3 = FString::Printf(TEXT("DeltaTime: %i"), Neighbors.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, Message3);
}

ACustomNavUnit* ACustomNavUnit::GetNextTarget(AActor* EndTarget)
{
	/*ACustomNavUnit* closestNeighbor = Neighbors[0];
	double closestDistance = (EndTarget->GetActorLocation() - closestNeighbor->GetActorLocation()).Size();*/

	ACustomNavUnit* closestNeighbor = nullptr;
	double closestDistance = 9999999.f;

	//for (ACustomNavUnit* Neighbor : Neighbors) {
		/*double distance = (Neighbor->GetActorLocation() - EndTarget->GetActorLocation()).Size();
		if (distance < closestDistance) {
			closestNeighbor = Neighbor;
			closestDistance = distance;
		}*/
	//}

	return closestNeighbor;
	//return nullptr;
}

// Called when the game starts or when spawned
void ACustomNavUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomNavUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

