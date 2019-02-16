// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleLaserTower.h"

// Sets default values
ASingleLaserTower::ASingleLaserTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//TowerObjectData.MeshComp->bEditableWhenInherited = true;
	//TowerObjectData.PlacementDecalComp->bEditableWhenInherited = true;

	TowerObjectData.PlacementRadius = 0.5;

	PlacementDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Comp"));
	
	FTransform DecalTransform = FTransform(
									FRotator(-90.0, 0.0, 0.0),	// Rotation
									FVector(0.0, 0.0, 0.0),		// Location
									FVector(1.0, TowerObjectData.PlacementRadius, TowerObjectData.PlacementRadius));	// Scale
	PlacementDecalComp->SetWorldTransform(DecalTransform);


	MeshComp->SetupAttachment(RootComponent);
	PlacementDecalComp->SetupAttachment(TowerObjectData.MeshComp);

	RootComponent = TowerObjectData.MeshComp;

	//RootComponent->bEditableWhenInherited = true;
	//this->AddInstanceComponent(MeshComp);
	//this->AddInstanceComponent(PlacementDecalComp);

	//TowerObjectData.MeshComp->RegisterComponent();

	TowerObjectData.Cost = 50;
}

// Called when the game starts or when spawned
void ASingleLaserTower::BeginPlay()
{
	Super::BeginPlay();
	
}

FTowerObjectData & ASingleLaserTower::GetDataStruct()
{
	// TODO: insert return statement here

	return TowerObjectData;
}

// Called every frame
void ASingleLaserTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

