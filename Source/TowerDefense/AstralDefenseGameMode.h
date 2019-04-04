// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AstralDefenseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AAstralDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAstralDefenseGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void resetTimer();

	FTimerHandle timer;
private:
	bool flag;
};
