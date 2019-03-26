// Fill out your copyright notice in the Description page of Project Settings.

#include "AITowerController.h"
#include "OneMissileTower.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAITowerController::AAITowerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	TowerTypeKeyName = "TowerType";
	TargetEnemyKeyName = "TargetEnemy";
	SelfActorKeyName = "SelfActor";

	bWantsPlayerState = true;
}


void AAITowerController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);

	AOneMissileTower* TowerBot = Cast<AOneMissileTower>(Pawn);
	if (TowerBot)
	{
		if (TowerBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*TowerBot->BehaviorTree->BlackboardAsset);

			/* Make sure the Blackboard has the type of bot we possessed */
			SetBlackboardTowerType(TowerBot->TowerType);
		}
		UE_LOG(LogTemp, Warning, TEXT("IN CONTROLLER"));

		BehaviorComp->StartTree(*TowerBot->BehaviorTree);
	}
}

void AAITowerController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

APawn * AAITowerController::GetTargetEnemy()
{
	if (Blackboard)
	{
		return Cast<APawn>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}
	return nullptr;
}

AOneMissileTower * AAITowerController::GetSelfActor()
{
	if (Blackboard)
	{
		return Cast<AOneMissileTower>(BlackboardComp->GetValueAsObject(SelfActorKeyName));
	}
	return nullptr;
}

void AAITowerController::SetSelfActor(AOneMissileTower * Self)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(SelfActorKeyName, Self);
	}
}

void AAITowerController::SetTargetEnemy(APawn * NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AAITowerController::SetBlackboardTowerType(ETowerBehaviorType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(TowerTypeKeyName, (uint8)NewType);
	}
}

/*
FRotator AAITowerController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}
*/

