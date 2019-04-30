// Fill out your copyright notice in the Description page of Project Settings.

#include "AILineTowerController.h"
#include "InLineTower.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAILineTowerController::AAILineTowerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAILineTowerController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	TowerTypeKeyName = "TowerType";
	TargetEnemyKeyName = "TargetEnemy";
	SelfActorKeyName = "SelfActor";

	bWantsPlayerState = true;
}


void AAILineTowerController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);

	AInLineTower* TowerBot = Cast<AInLineTower>(Pawn);
	if (TowerBot)
	{
		if (TowerBot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*TowerBot->BehaviorTree->BlackboardAsset);

			// Make sure the Blackboard has the type of bot we possessed 
			SetBlackboardTowerType(TowerBot->TowerObjectData.TowerType);
		}
		UE_LOG(LogTemp, Warning, TEXT("IN CONTROLLER"));

		BehaviorComp->StartTree(*TowerBot->BehaviorTree);

		SightConfig->SightRadius = TowerBot->TowerObjectData.AttackRadius;
	}
}

void AAILineTowerController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AAILineTowerController::BeginPlay()
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

void AAILineTowerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentPawns.Num() > 0)
	{
		if (!GetTargetEnemy() && CurrentTarget)
		{
			if (!GetTargetEnemy() && CurrentTarget)	
			{
				CurrentPawns.Remove(CurrentTarget);
			}

			if (GetTargetEnemy() == nullptr && !CurrentTarget)
			{
				APawn* SensedPawn = Cast<APawn>(CurrentPawns[0]);

				if (TargetMesh)
				{
					TargetMesh->SetRenderCustomDepth(true);
				}
				CurrentTarget = SensedPawn;
				SetTargetEnemy(SensedPawn);
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("^^^^^^^^^^"));
		//for (auto& Actor : CurrentPawns)
		{
			//APawn* EnemyPawn = Cast<APawn>(Actor);
			//UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), *EnemyPawn->GetName());
		}
		//UE_LOG(LogTemp, Warning, TEXT("VVVVVVVVVVVV"));
		//UE_LOG(LogTemp, Warning, TEXT("VVVVVVVVVVVV"));
	}
	else if (bCheckOnce == false)
	{
		AInLineTower* Tower = Cast<AInLineTower>(GetPawn());
		Tower->TowerObjectData.bSensedTarget = false;
		SetBlackboardTowerType(ETowerBehaviorType::Inactive);
		bCheckOnce = true;
	}
}

void AAILineTowerController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	//UE_LOG(LogTemp, Warning, TEXT("In Pawn Detected"));
	AInLineTower* Tower = Cast<AInLineTower>(GetPawn());
	FString Label = DetectedPawns[0]->GetActorLabel();
	if (Label == "Enemy" && Tower->TowerObjectData.bPlaced && CurrentPawns.Find(DetectedPawns[0]) == -1)
	{

		//UE_LOG(LogTemp, Warning, TEXT("Add Pawn"));
		bCheckOnce = false;
		CurrentPawns.Add(DetectedPawns[0]);
		if (!Tower->TowerObjectData.bSensedTarget)
		{
			//BroadcastUpdateAudioLoop(true);
		}

		//Keep track of the time the player was last sensed in order to clear the target 
		Tower->TowerObjectData.LastSeenTime = GetWorld()->GetTimeSeconds();
		Tower->TowerObjectData.bSensedTarget = true;


		if (GetTargetEnemy() == nullptr)
		{
			APawn* SensedPawn = Cast<APawn>(CurrentPawns[0]);
			TArray<UStaticMeshComponent*> StaticComps;
			AActor* TargetActor = Cast<AActor>(SensedPawn);
			TargetActor->GetComponents<UStaticMeshComponent>(StaticComps);
			
			TargetMesh = StaticComps[0];
			if (TargetMesh)
			{
				TargetMesh->SetRenderCustomDepth(true);
			}
			CurrentTarget = SensedPawn;

			SetTargetEnemy(SensedPawn);
			SetBlackboardTowerType(ETowerBehaviorType::Active);
			SetSelfActor(Tower);
		}
	}
	else if (Label == "Enemy" && Tower->TowerObjectData.bPlaced)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Remove Pawn"));
		CurrentPawns.Remove(DetectedPawns[0]);
		if (TargetMesh)
		{
			TargetMesh->SetRenderCustomDepth(false);
		}
		CurrentTarget = nullptr;
		SetTargetEnemy(nullptr);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Out Pawn Detected"));





}

APawn * AAILineTowerController::GetTargetEnemy()
{
	if (BlackboardComp)
	{
		return Cast<APawn>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}
	return nullptr;
}

AInLineTower * AAILineTowerController::GetSelfActor()
{
	if (BlackboardComp)
	{
		return Cast<AInLineTower>(BlackboardComp->GetValueAsObject(SelfActorKeyName));
	}
	return nullptr;
}

void AAILineTowerController::SetSelfActor(AInLineTower * Self)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(SelfActorKeyName, Self);
	}
}

void AAILineTowerController::SetTargetEnemy(APawn * NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AAILineTowerController::SetBlackboardTowerType(ETowerBehaviorType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(TowerTypeKeyName, (uint8)NewType);
	}


}

// Current Target Killed Returns Next Target		
APawn * AAILineTowerController::TargetWasKilled()
{
	CurrentPawns.RemoveAt(0);
	if (CurrentPawns.Num() > 0)
	{
		APawn* NewSensedPawn = Cast<APawn>(CurrentPawns[0]);
		CurrentTarget = NewSensedPawn;
		SetTargetEnemy(NewSensedPawn);

		return NewSensedPawn;
	}


	return nullptr;
}

FRotator AAILineTowerController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}


