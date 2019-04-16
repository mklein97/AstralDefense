// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TowerInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../AstralDefensePlayerController.h"
#include "OneMissileTower.generated.h"

class ASingleLaserProjectile;
class USoundCue;
class UAudioComponent;
class AAITowerController;
UCLASS()
class TOWERDEFENSE_API AOneMissileTower : public APawn, public ITowerInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Properties")
	FTowerObjectData TowerObjectData;

	APlayerController* PlayerController;
	///
	/*
	float LastSeenTime;

	float LastHeardTime; // For When Noise is implemented

	float LastAttackTime;

		float SenseTimeOut;

	bool bSensedTarget;
	*/
	///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MaterialPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComp;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<ASingleLaserProjectile> ProjectileClass;

	///
	/*
	APlayerController* PlayerController;
	FVector MouseLocation;
	FVector MouseDirection;
	*/
	///

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* FireSound;
protected:
	UMaterialInstance* PlacementMat;
	UMaterialInstance* UnableMat;
	UMaterialInstance* AttackRadMat;
	//virtual bool IsSprinting() const override;

	/* Triggered by pawn sensing component when a pawn is spotted */


	//UPROPERTY(VisibleAnywhere, Category = "Attacking")
	//UCapsuleComponent* MeleeCollisionComp;

	/* A pawn is in melee range */
	//UFUNCTION()
	//void OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//void OnRetriggerMeleeStrike();

	/* Deal damage to the Actor that was hit by the punch animation */
	//UFUNCTION(BlueprintCallable, Category = "Attacking")
	//void PerformMeleeStrike(AActor* HitActor);

	//UFUNCTION(Reliable, NetMulticast)
	//void SimulateMeleeStrike();

	//void SimulateMeleeStrike_Implementation();

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		TSubclassOf<UDamageType> MissileDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MissileDamage;

	//UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	//UAnimMontage* MeleeAnimMontage;

	/* Update the vocal loop of the zombie (idle, wandering, hunting) */
	//UFUNCTION(Reliable, NetMulticast)
	//void BroadcastUpdateAudioLoop(bool bNewSensedTarget);

	//void BroadcastUpdateAudioLoop_Implementation(bool bNewSensedTarget);

	UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundPlayerNoticed;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundAttackFire;



	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MissileAttack;

	/* Minimum time between melee attacks */
	float MissileStrikeCooldown;

	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* AudioLoopComp;

	// UNCOMMENT
	//void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled) override;




public:
	FTowerObjectData* GetDataStruct();
	void SetPlaced();
	int32 TowerCost(int32 CurrentStarbucks);

	void SetSelected();
	void DisableAttackRadiusDecal();
	void EnableAttackRadiusDecal();
	bool IsCollidingWith(ITowerInterface &otherActor);

	UFUNCTION()
		void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult& Hit);

	UFUNCTION()
		void OffCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets default values for this pawn's properties

	AOneMissileTower(const class FObjectInitializer& ObjectInitializer);

	///
	/*
	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
		bool bIsFiring;

	UPROPERTY(EditAnywhere, Category = "AI")
		ETowerBehaviorType TowerType;
	*/
	///

	/* The thinking part of the brain, steers our zombie and makes decisions based on the data we feed it from the Blackboard */
	/* Assigned at the Character level (instead of Controller) so we may use different zombie behaviors while re-using one controller. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	/* Change default bot type during gameplay */
	void SetTowerType(ETowerBehaviorType NewType);


	/////////////////////////////////////

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};