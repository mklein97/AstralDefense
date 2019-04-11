// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TowerInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../AstralDefensePlayerController.h"
#include "InLineTower.generated.h"

class USoundCue;
class UAudioComponent;
class AAITowerController;
class UParticleSystem;
UCLASS()
class TOWERDEFENSE_API AInLineTower : public APawn, public ITowerInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Properties")
		FTowerObjectData TowerObjectData;

	APlayerController* PlayerController;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MaterialPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComp;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		UParticleSystem* Particle;
	UParticleSystem* TempParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* FireSound;
protected:
	UMaterialInstance* PlacementMat;
	UMaterialInstance* UnableMat;
	UMaterialInstance* AttackRadMat;




	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		TSubclassOf<UDamageType> MissileDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MissileDamage;


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
	float TowerCost(float CurrentStarbucks);

	void SetSelected();
	void DisableAttackRadiusDecal();
	void EnableAttackRadiusDecal();

	UFUNCTION()
		void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult& Hit);

	UFUNCTION()
		void OffCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets default values for this pawn's properties

	AInLineTower(const class FObjectInitializer& ObjectInitializer);


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
