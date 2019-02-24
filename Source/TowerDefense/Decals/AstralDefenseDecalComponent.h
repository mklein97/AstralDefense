// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "AstralDefenseDecalComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UAstralDefenseDecalComponent : public UDecalComponent
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = "ADDecal")
		void SetDecalSize(FVector Size);
};
