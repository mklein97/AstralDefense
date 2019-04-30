// Fill out your copyright notice in the Description page of Project Settings.
#define COLLISION_TOWERS		ECC_GameTraceChannel2

#include "AstralDefensePlayerController.h"
#include "Blueprint//AIBlueprintHelperLibrary.h"
#include "Towers/OneMissileTower.h"
#include "Towers/InLineTower.h"
#include "Engine/Public/EngineUtils.h"
#include "Engine/Engine.h"
#include "Engine/Blueprint.h"
#include "DrawDebugHelpers.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "../PlayerBase.h"

AAstralDefensePlayerController::AAstralDefensePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;


}


void AAstralDefensePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor && PlacingTower)
	{
		PlaceTower();
		bMoveToMouseCursor = false;
	}
	else
	{
		bMoveToMouseCursor = false;
	}
}

void AAstralDefensePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAstralDefensePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAstralDefensePlayerController::OnSetDestinationReleased);

}

void AAstralDefensePlayerController::PlaceTower()
{
	//UE_LOG(LogTemp, Warning, TEXT("Relative Location, %d, %d, %d"), PlacementDecalComp->getlocation );
	//UE_LOG(LogTemp, Warning, TEXT("In Place Tower"));



	if (PlacingTower)
	{
		//FTowerSearch Tower2Array;
		//Tower2Array.ITower = PlacingTower;
		//Tower2Array.bplaced = false;
		//Towers->Add(Tower2Array);

		//UE_LOG(LogTemp, Warning, TEXT("Placing Tower Name: %s"), *PlacingTower->GetName());
		UE_LOG(LogTemp, Warning, TEXT("|"));
		for (TActorIterator<AOneMissileTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			//ASingleLaserTower *Tower = *ActorItr;
			AOneMissileTower *Tower = *ActorItr;

			FTransform LocalToWorld = FTransform(
				FRotator(0, 0, 0),
				Tower->GetActorLocation(),
				FVector(1, 1, 1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);

			AOneMissileTower* TempPlaceTower = Cast<AOneMissileTower>(PlacingTower);
			if (TempPlaceTower)
			{
				if (Tower == TempPlaceTower)
				{
					UE_LOG(LogTemp, Warning, TEXT("EQUAL!"));

					if (Tower->TowerObjectData.bCollidesToggle)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Attempting to Place Tower."));

						Tower->SetPlaced();
						///
						UE_LOG(LogTemp, Warning, TEXT("Start Looking for Base"));
						for (TActorIterator<APlayerBase> ActorItr(GetWorld()); ActorItr; ++ActorItr)
						{
							UE_LOG(LogTemp, Warning, TEXT("Looking for Base"));

							// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
							APlayerBase* player = *ActorItr;
							if (player != nullptr)
							{
								UE_LOG(LogTemp, Warning, TEXT("Base Money: %d"), player->Money);
								player->Money -= 20;// Tower->TowerCost(player->Money);
								UE_LOG(LogTemp, Warning, TEXT("Base Money: %d"), player->Money);

							}
							break;
						}
						///

						//Tower2Array.bplaced = true;
						PlacingTower = nullptr;
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attempting to Place Tower."));
					}
					//UE_LOG(LogTemp, Warning, TEXT("COMPARE TOWER: %s"), *Tower->GetName());
				}

			}

		}

		for (TActorIterator<AInLineTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			//ASingleLaserTower *Tower = *ActorItr;
			AInLineTower *Tower = *ActorItr;

			FTransform LocalToWorld = FTransform(
				FRotator(0, 0, 0),
				Tower->GetActorLocation(),
				FVector(1, 1, 1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);

			AInLineTower* TempPlaceTower = Cast<AInLineTower>(PlacingTower);
			if (TempPlaceTower)
			{
				if (Tower == TempPlaceTower)
				{
					UE_LOG(LogTemp, Warning, TEXT("EQUAL!"));

					if (Tower->TowerObjectData.bCollidesToggle)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Attempting to Place Tower."));

						Tower->SetPlaced();

						///
						UE_LOG(LogTemp, Warning, TEXT("Start Looking for Base"));
						for (TActorIterator<APlayerBase> ActorItr(GetWorld()); ActorItr; ++ActorItr)
						{
							UE_LOG(LogTemp, Warning, TEXT("Looking for Base"));

							// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
							APlayerBase* player = *ActorItr;
							if (player != nullptr)
							{
								player->Money = Tower->TowerCost(player->Money);
								UE_LOG(LogTemp, Warning, TEXT("Base Health: %f"), player->Money);

							}
							break;
						}
						///

						PlacingTower = nullptr;
					}
					else
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attempting to Place Tower."));
					}
					//UE_LOG(LogTemp, Warning, TEXT("COMPARE TOWER: %s"), *Tower->GetName());
				}

			}

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("|"));

}




void AAstralDefensePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	//bMoveToMouseCursor = true;
}

void AAstralDefensePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = true;
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	FHitResult Selection;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(COLLISION_TOWERS),false,Selection);

	if (Hit.bBlockingHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Works"));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

	}
	if (Selection.bBlockingHit)
	{
		//DrawDebugLine(GetWorld(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), Selection.GetActor()->GetActorLocation(), FColor::Red, false, 1, 0, 2.f);

		//UE_LOG(LogTemp, Warning, TEXT("Found Tower!"));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Selection.GetActor()->GetName());

		if (SelectedTower)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found Tower!"));

			FTowerObjectData* TowerOD = SelectedTower->GetDataStruct();
			TowerOD->bSelected = false;
			// We hit something, move there
			AOneMissileTower* Tower = Cast<AOneMissileTower>(Selection.GetActor());
			SelectedTower = Cast<ITowerInterface>(Tower);
			if (SelectedTower)
			{
				Tower->SetSelected();
			}
			else
			{
				AInLineTower* Tower = Cast<AInLineTower>(Selection.GetActor());
				SelectedTower = Cast<ITowerInterface>(Tower);
				if (SelectedTower)

					Tower->SetSelected();
			}
		}
		else
		{

			// We hit something, move there
			AOneMissileTower* Tower = Cast<AOneMissileTower>(Selection.GetActor());
			SelectedTower = Cast<ITowerInterface>(Tower);
			if (SelectedTower)
			{
				Tower->SetSelected();
			}
			else
			{
				AInLineTower* Tower = Cast<AInLineTower>(Selection.GetActor());
				SelectedTower = Cast<ITowerInterface>(Tower);
				if (SelectedTower)

					Tower->SetSelected();
			}
		}
	}

}

/*
void AAstralDefensePlayerController::SetPlacingTower(ASingleLaserTower * Tower)
{
	this->PlacingTower = Tower;
}*/

void AAstralDefensePlayerController::SetPlacingTower(ITowerInterface * Tower)
{
	this->PlacingTower = Tower;
}

void AAstralDefensePlayerController::Tick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// CHECK IF PERFORMANCE ISSUE
	if (PlacingTower)// && PlacingTower->TowerObjectData.bPlacing)
	{
		for (TActorIterator<AOneMissileTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			//ASingleLaserTower *Tower = *ActorItr;
			AOneMissileTower *Tower = *ActorItr;

			FTransform LocalToWorld = FTransform(
				FRotator(0, 0, 0),
				Tower->GetActorLocation(),
				FVector(1, 1, 1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);


			//ClientMessage(ActorItr->GetName());
			//ClientMessage(ActorItr->GetActorLocation().ToString());
		}
		for (TActorIterator<AInLineTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			//ASingleLaserTower *Tower = *ActorItr;
			AInLineTower *Tower = *ActorItr;

			FTransform LocalToWorld = FTransform(
				FRotator(0, 0, 0),
				Tower->GetActorLocation(),
				FVector(1, 1, 1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);


			//ClientMessage(ActorItr->GetName());
			//ClientMessage(ActorItr->GetActorLocation().ToString());
		}
	}
}
