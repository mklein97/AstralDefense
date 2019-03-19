// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITowerController.generated.h"

class UBehaviorTreeComponent;
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

	virtual FRotator GetControlRotation() const override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TowerTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyNamel;

public:
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRadius = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;
	
};
