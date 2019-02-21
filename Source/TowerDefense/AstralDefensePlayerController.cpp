// Fill out your copyright notice in the Description page of Project Settings.

#include "AstralDefensePlayerController.h"
#include "Towers/SingleLaserTower.h"
#include "Blueprint//AIBlueprintHelperLibrary.h"
#include "Engine/Public/EngineUtils.h"

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
	UE_LOG(LogTemp, Warning, TEXT("In Place Tower"));

	for (TActorIterator<ASingleLaserTower> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ASingleLaserTower *Tower = *ActorItr;
		Tower->SetPlaced();
		UE_LOG(LogTemp, Warning, TEXT("Tower Name: %s"), *Tower->GetName());
		//ClientMessage(ActorItr->GetName());
		//ClientMessage(ActorItr->GetActorLocation().ToString());
	}
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


