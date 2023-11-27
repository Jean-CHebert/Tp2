// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameStatsActor.h"
#include "Components/GameStatsComponent.h"

// Sets default values
AGameStatsActor::AGameStatsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameStatsComponent = CreateDefaultSubobject<UGameStatsComponent>(TEXT("Game Stats Component"));
}

// Called when the game starts or when spawned
void AGameStatsActor::BeginPlay()
{
	Super::BeginPlay();
	
}

float AGameStatsActor::GetEnemyCount()
{
	return GameStatsComponent->GetEnemyCount();
}

void AGameStatsActor::EnemyDead()
{
	GameStatsComponent->EnemyDead();
}

// Called every frame
void AGameStatsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

