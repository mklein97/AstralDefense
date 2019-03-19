// Fill out your copyright notice in the Description page of Project Settings.

#include "OneMissileTower.h"
#include "Kismet/GameplayStatics.h"

AOneMissileTower::AOneMissileTower(const FObjectInitializer & ObjectInitializer)
{
}

void AOneMissileTower::SetTowerType(ETowerBehaviorType NewType)
{
}

// Sets default values
AOneMissileTower::AOneMissileTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOneMissileTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOneMissileTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOneMissileTower::OnSeenPawn(APawn * Pawn)
{
}

void AOneMissileTower::OnHearNoise(APawn * PawnInstigator, const FVector & Location, float Volume)
{
}

UAudioComponent * AOneMissileTower::PlayCharacterSound(USoundCue * CueToPlay)
{
	return nullptr;
}

// Called to bind functionality to input
void AOneMissileTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

