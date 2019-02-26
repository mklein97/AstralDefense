// Fill out your copyright notice in the Description page of Project Settings.

#include "BeamTower.h"

// Sets default values
ABeamTower::ABeamTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeamTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeamTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

