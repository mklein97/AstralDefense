// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "../Public/STypes.h"
#include "AITowerController.h"
#include "AILineTowerController.h"
#include "../AstralDefensePlayerController.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;
	
	int Kills;

	bool bPlaced = false;
	bool bSelected = false;

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

	AAITowerController* AITController;
	AAILineTowerController* AILineController;
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

	/// Tower Class
	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime; // For When Noise is implemented

	/* Last time we attacked something */
	float LastAttackTime;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	float SenseTimeOut;

	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	bool bSensedTarget;

	AAstralDefensePlayerController* PlayerController;
	FVector MouseLocation;
	FVector MouseDirection;

	//UPROPERTY(BlueprintReadWrite, Category = "Attacking")
	bool bIsFiring;

	/* The tower behavior we want this bot to execute, (passive/patrol) by specifying EditAnywhere we can edit this value per-instance when placed on the map. */
	//UPROPERTY(EditAnywhere, Category = "AI")
	ETowerBehaviorType TowerType;


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
	//virtual bool IsCollidingWith(ITowerInterface &otherActor) = 0;
	/* Change default bot type during gameplay */
	virtual void SetTowerType(ETowerBehaviorType NewType) = 0;
	virtual void SetSelected() = 0;
	//UFUNCTION(BlueprintCallable)
	virtual int32 TowerCost(int32 CurrentStarbucks) = 0;
	virtual void DisableAttackRadiusDecal() = 0;
	virtual void EnableAttackRadiusDecal() = 0;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);


};
