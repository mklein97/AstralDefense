// Fill out your copyright notice in the Description page of Project Settings.

#include "AstralDefenseGameMode.h"
#include "AstralDefensePlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase.h"
#include "Engine.h"

AAstralDefenseGameMode::AAstralDefenseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAstralDefensePlayerController::StaticClass();

	UE_LOG(LogTemp, Warning, TEXT("In AstralGM"));

	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
	flag = false;
}

void AAstralDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AAstralDefenseGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundActors);

	for (TActorIterator<APlayerBase> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		APlayerBase* player = *ActorItr;
		if (player != nullptr)
		{
			if (player->Health <= 0)
			{
				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("DEATH")));
				if (!flag)
				{
					GetWorldTimerManager().SetTimer(timer, this, &AAstralDefenseGameMode::resetTimer, 5.0f, false);
					flag = true;
				}
			}
		}
	}
}

void AAstralDefenseGameMode::resetTimer()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	GetWorldTimerManager().ClearTimer(timer);
}
