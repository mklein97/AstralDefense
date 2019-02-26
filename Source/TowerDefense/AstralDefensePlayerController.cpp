// Fill out your copyright notice in the Description page of Project Settings.

#include "AstralDefensePlayerController.h"
#include "Blueprint//AIBlueprintHelperLibrary.h"
#include "Towers/SingleLaserTower.h"
#include "Engine/Public/EngineUtils.h"
#include "Engine/Engine.h"

AAstralDefensePlayerController::AAstralDefensePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAstralDefensePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		PlaceTower();
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
		UE_LOG(LogTemp, Warning, TEXT("Placing Tower Name: %s"), *PlacingTower->GetName());
		UE_LOG(LogTemp, Warning, TEXT("|"));
		for (TActorIterator<ASingleLaserTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ASingleLaserTower *Tower = *ActorItr;
			FTransform LocalToWorld = FTransform(
				FRotator(0,0,0),
				Tower->GetActorLocation(),
				FVector(1,1,1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);
			if (Tower == PlacingTower)
			{

				if (Tower->TowerObjectData.bCollidesToggle)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Attempting to Place Tower."));

					Tower->SetPlaced();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attempting to Place Tower."));
				}
				//UE_LOG(LogTemp, Warning, TEXT("COMPARE TOWER: %s"), *Tower->GetName());
			}
			else
			{
				ITowerInterface* Actor2Interface = Cast<ITowerInterface>(Tower);
				if (Actor2Interface)
				{
					if (PlacingTower->IsCollidingWith(*Actor2Interface))
					{
						UE_LOG(LogTemp, Warning, TEXT("COLLIDING!"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("NOT COLLIDING!"));
					}
				}
				//Tower->SetPlaced();
				UE_LOG(LogTemp, Warning, TEXT("NonPlacing Tower Name: %s"), *Tower->GetName());
			}

			//ClientMessage(ActorItr->GetName());
			//ClientMessage(ActorItr->GetActorLocation().ToString());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("|"));

}

void AAstralDefensePlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}

}

void AAstralDefensePlayerController::SetNewMoveDestination(const FVector DestLocation)
{

	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
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
}

void AAstralDefensePlayerController::SetPlacingTower(ASingleLaserTower* Tower)
{
	this->PlacingTower = Tower;
}

void AAstralDefensePlayerController::Tick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (PlacingTower && PlacingTower->TowerObjectData.bPlacing)
	{
		for (TActorIterator<ASingleLaserTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ASingleLaserTower *Tower = *ActorItr;
			FTransform LocalToWorld = FTransform(
				FRotator(0, 0, 0),
				Tower->GetActorLocation(),
				FVector(1, 1, 1)
			);

			Tower->TowerObjectData.CollisionBounds = Tower->CollisionComp->CalcBounds(LocalToWorld);

			if (Tower == PlacingTower)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Placing Tower Name: %s"), *Tower->GetName());
				//Tower->SetPlaced();
			}
			else
			{
				ITowerInterface* Actor2Interface = Cast<ITowerInterface>(Tower);
				if (Actor2Interface )
				{
					if (PlacingTower->IsCollidingWith(*Actor2Interface))
					{
						//UE_LOG(LogTemp, Warning, TEXT("COLLIDING!"));
					}

				}
				
				//Tower->SetPlaced();
				//UE_LOG(LogTemp, Warning, TEXT("NonPlacing Tower Name: %s"), *Tower->GetName());
			}
			//ClientMessage(ActorItr->GetName());
			//ClientMessage(ActorItr->GetActorLocation().ToString());
		}
	}
}
