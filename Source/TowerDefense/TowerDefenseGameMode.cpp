// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseGameMode.h"
#include "TowerDefensePlayerController.h"
#include "TowerDefenseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATowerDefenseGameMode::ATowerDefenseGameMode()
{
	/*
	 use our custom PlayerController class
	PlayerControllerClass = ATowerDefensePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}