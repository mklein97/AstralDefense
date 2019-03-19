// Fill out your copyright notice in the Description page of Project Settings.

#include "AITowerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAITowerController::AAITowerController()
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

}

void AAITowerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Ocurred"));
	}
}

void AAITowerController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);
}

void AAITowerController::UnPossess()
{
}

void AAITowerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FRotator AAITowerController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAITowerController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	UE_LOG(LogTemp, Warning, TEXT("FOUND SOMETHING"));
}
