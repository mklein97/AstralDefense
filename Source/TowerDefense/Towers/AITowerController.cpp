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
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAITowerController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

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

void AAITowerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set for AI Tower Controller"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Tower Controller has failed"));
	}
}

void AAITowerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentPawns.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("^^^^^^^^^^"));
		for (auto& Actor : CurrentPawns)
		{
			APawn* EnemyPawn = Cast<APawn>(Actor);
			UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), *EnemyPawn->GetName());
		}
		UE_LOG(LogTemp, Warning, TEXT("VVVVVVVVVVVV"));
	}
}

void AAITowerController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	AOneMissileTower* Tower = Cast<AOneMissileTower>(GetPawn());
	if(CurrentPawns.is)
	CurrentPawns.Add(DetectedPawns[0]);

	/*
	if (!Tower->bSensedTarget)
	{
		//BroadcastUpdateAudioLoop(true);
	}

	//Keep track of the time the player was last sensed in order to clear the target 
	Tower->LastSeenTime = GetWorld()->GetTimeSeconds();
	Tower->bSensedTarget = true;

	APawn* SensedPawn = Cast<APawn>(CurrentPawns[0]);

	if (GetTargetEnemy() == nullptr)
	{
		SetTargetEnemy(SensedPawn);
		SetBlackboardTowerType(ETowerBehaviorType::Active);
		SetSelfActor(Tower);
	}
	*/

	UE_LOG(LogTemp, Warning, TEXT("On Pawn Detected"));

	for (auto& Actor : CurrentPawns)
	{
		APawn* EnemyPawn = Cast<APawn>(Actor);
		UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), *EnemyPawn->GetName());
	}


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



FRotator AAITowerController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}


