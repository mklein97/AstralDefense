// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerInterface.h"

// Add default functionality here for any ITowerInterface functions that are not pure virtual.

/*bool ITowerInterface::IsPlacing()
{
	FTowerObjectData TowerOD = GetDataStruct();
	if (TowerOD.bPlacing)
		return true;
	else
		return false;
}*/

void ITowerInterface::SetPlaced()
{
	FTowerObjectData* TowerOD = GetDataStruct();
	TowerOD->bPlaced = true;
	TowerOD->bPlacing = false;
	//bool test = TowerOD->bPlaced;

	//UE_LOG(LogTemp, Warning, TEXT("In Interface: %s"), (test ? TEXT("true") : TEXT("false")) );
	
	//test = TowerOD->bPlaced;
	//UE_LOG(LogTemp, Warning, TEXT("In Interface: %s"), (test ? TEXT("true") : TEXT("false")) );
	//test = true;
	//UE_LOG(LogTemp, Warning, TEXT("In Interface: %s"), (test ? TEXT("true") : TEXT("false")) );

}