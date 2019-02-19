// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TowerInterface.h"
#include "SingleLaserTower.generated.h"

UCLASS()
class TOWERDEFENSE_API ASingleLaserTower : public AActor, public ITowerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleLaserTower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Properties")
	FTowerObjectData TowerObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDecalComponent* PlacementDecalComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float testfloat;

	APlayerController* PlayerController;
	FVector MouseLocation;
	FVector MouseDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FTowerObjectData& GetDataStruct();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
