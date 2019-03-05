// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AstralDefensePlayerController.generated.h"

class ASingleLaserTower;
/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AAstralDefensePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAstralDefensePlayerController();
	void SetPlacingTower(ASingleLaserTower* Tower);

	virtual void Tick(float DeltaTime) override;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	ASingleLaserTower* PlacingTower;



	void PlaceTower();
};
