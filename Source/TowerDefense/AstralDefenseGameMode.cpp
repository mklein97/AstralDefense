// Fill out your copyright notice in the Description page of Project Settings.

#include "AstralDefenseGameMode.h"
#include "TowerDefensePlayerController.h"
#include "TowerDefenseCharacter.h"
#include "UObject/ConstructorHelpers.h"


AAstralDefenseGameMode::AAstralDefenseGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATowerDefensePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAstralDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld->GetFirstPlayerController()->bShowMouseCursor = true;
}