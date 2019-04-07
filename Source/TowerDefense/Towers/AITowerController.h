// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Public/STypes.h"
#include "AITowerController.generated.h"

class UBehaviorTreeComponent;
class AOneMissileTower;
/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AAITowerController : public AAIController
{
	GENERATED_BODY()

public:
	AAITowerController(const FObjectInitializer& ObjectInitializer);
	
	virtual void Possess(APawn* Pawn) override;
	
	virtual void UnPossess() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;


	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TowerTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName SelfActorKeyName;


public:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	APawn* GetTargetEnemy();
	
	AOneMissileTower* GetSelfActor();

	void SetSelfActor(AOneMissileTower* Self);

	void SetTargetEnemy(APawn* NewTarget);

	void SetBlackboardTowerType(ETowerBehaviorType NewType);


	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 1200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 180.0f;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	virtual FRotator GetControlRotation() const override;

	TArray<AActor*> CurrentPawns;
	UStaticMeshComponent* TargetMesh;
	bool bCheckOnce;
};
