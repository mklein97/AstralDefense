// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "../Public/STypes.h"
#include "TowerInterface.generated.h"
USTRUCT(BlueprintType)
struct FTowerObjectData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlacementRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LaserFireRate;

	bool Reloading;

	int Cost;
	int Kills;

	bool bPlaced = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPlacing = false;
	bool bCollidesToggle = true;
	bool bNotCollidesToggle = false;

	FBoxSphereBounds DecalBounds;
	FBoxSphereBounds MeshBounds;
	FBoxSphereBounds CollisionBounds;

	FVector ActorLocation;

	UMaterialInstanceDynamic* PlacementMatInst;
	UMaterialInstanceDynamic* UnableMatInst;
	UMaterialInstanceDynamic* AttackRadMatInst;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	//UStaticMeshComponent* MeshComp;
	APawn* CurrentTarget;
	bool bIsTargeting;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	//UDecalComponent* PlacementDecalComp;

	//UPROPERTY(EditAnywhere)
	//UDecalComponent* AttackDecalComp;

	TArray<APawn*> AllTargets;

	/************************************************************************/
	/* Damage & Death                                                       */
	/************************************************************************/

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float Health;


	/* Holds hit data to replicate hits and death to clients */
	UPROPERTY(EditAnywhere)
		struct FTakeHitInfo LastTakeHitInfo;
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

	virtual FTowerObjectData* GetDataStruct() = 0;

	//virtual bool IsPlacing() ;
	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	virtual void SetPlaced() = 0;
	virtual void DisableAttackRadiusDecal() = 0;
	virtual bool IsCollidingWith(ITowerInterface &otherActor) = 0;



	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);


};
