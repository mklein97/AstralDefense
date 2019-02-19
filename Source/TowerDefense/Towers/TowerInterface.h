// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "TowerInterface.generated.h"
USTRUCT(BlueprintType)
struct FTowerObjectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlacementRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius;

	int Cost;
	int Kills;

	bool bPlaced = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlacing = false;
	bool bCollidesWith = false;


	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	//UStaticMeshComponent* MeshComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	//UDecalComponent* PlacementDecalComp;

	//UPROPERTY(EditAnywhere)
	//UDecalComponent* AttackDecalComp;
};
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTowerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWERDEFENSE_API ITowerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual FTowerObjectData& GetDataStruct() = 0;

	virtual bool IsPlacing() = 0;
};
