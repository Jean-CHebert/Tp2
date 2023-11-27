// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

// Sets default values for this component's properties
UGameStatsComponent::UGameStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGameStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TArray<AActor*> enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemies);
	EnemyCount = enemies.Num();
	
}


// Called every frame
void UGameStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGameStatsComponent::EnemyDead_Implementation()
{
	EnemyCount -= 1;
	if (EnemyCount <= 0) {
		AActor* parent = GetOwner();
		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("VictoryScreen"));
		parent->CallFunctionByNameWithArguments(*command, ar, NULL, true);

		//UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
	}
}

