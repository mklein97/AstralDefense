// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Public/TimerManager.h"

#include "BTTask_FireAtTarget.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UBTTask_FireAtTarget : public UBTTaskNode
{
	GENERATED_BODY()

		FTimerHandle ParticleDespawnHandle;
	FTimerDelegate TimerDel;

	UFUNCTION()
		void ToDespawn(UParticleSystemComponent* Particle,UAudioComponent* Audio);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
