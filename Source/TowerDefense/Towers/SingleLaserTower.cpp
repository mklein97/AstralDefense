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
									FRotator(90.0, 0.0, 0.0),	// Rotation
									FVector(0.0, 0.0, 0.0),		// Location
									FVector(1.0, TowerObjectData.PlacementRadius, TowerObjectData.PlacementRadius));	// Scale
	PlacementDecalComp->SetRelativeTransform(DecalTransform);
	//PlacementDecalComp->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	//PlacementDecalComp->SetRelativeScale3D(FVector(1.0, TowerObjectData.PlacementRadius, TowerObjectData.PlacementRadius));
	//PlacementDecalComp->SetRelativeRotation(FRotator(90.0, 0.0,))


	MeshComp->SetupAttachment(RootComponent);
	PlacementDecalComp->SetupAttachment(MeshComp);

	RootComponent = MeshComp;

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
	
	PlayerController = GetWorld()->GetFirstPlayerController();

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

	
	if (PlayerController != nullptr)
	{
		if (!TowerObjectData.bPlaced )
		{
			FHitResult TraceHitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			this->SetActorLocation(TraceHitResult.Location);
			this->SetActorRotation(this->GetActorRotation());
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Relative Location, %d, %d, %d"), PlacementDecalComp->getlocation );
}
