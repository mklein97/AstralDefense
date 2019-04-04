// Fill out your copyright notice in the Description page of Project Settings.

#include "STowerState.h"
#include "Net/UnrealNetwork.h"

ASTowerState::ASTowerState(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	TeamNumber = 0;
}
void ASTowerState::Reset()
{
	Super::Reset();

	NumKills = 0;
	NumDeaths = 0;
	Score = 0;
}

void ASTowerState::AddKill()
{
	NumKills++;
}

void ASTowerState::AddDeath()
{
	NumDeaths++;
}

void ASTowerState::ScorePoints(int32 Points)
{
	Score += Points;

	// Add Score to Global score in Game State
}

void ASTowerState::SetTeamNumber(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

int32 ASTowerState::GetTeamNumber() const
{
	return TeamNumber;
}

int32 ASTowerState::GetKills() const
{
	return NumKills;
}

int32 ASTowerState::GetDeaths() const
{
	return NumDeaths;
}

float ASTowerState::GetScore() const
{
	return Score;
}

void ASTowerState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTowerState, NumKills);
	DOREPLIFETIME(ASTowerState, NumDeaths);
	DOREPLIFETIME(ASTowerState, TeamNumber);
}