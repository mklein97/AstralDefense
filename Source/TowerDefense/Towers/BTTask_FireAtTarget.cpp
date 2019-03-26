// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FireAtTarget.h"

#include "AITowerController.h"

#include "Kismet/GameplayStatics.h"
#include "../Projectiles/SingleLaserProjectile.h"
#include "OneMissileTower.h"
/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"
EBTNodeResult::Type UBTTask_FireAtTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAITowerController* MyController = Cast<AAITowerController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	APawn* Target = MyController->GetTargetEnemy();
	AOneMissileTower* Tower = MyController->GetSelfActor();
	if (Target)
	{
		FVector TLocation = MyController->GetTargetLocation(Target);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Attempting to Fire Laser"));

		// try and fire a projectile
		if (Tower->ProjectileClass)
		{
			//FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
			//FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
			FRotator TowerRotation = Tower->GetActorRotation();
			FVector TowerLocation = Tower->GetActorLocation() + TowerRotation.Vector().Normalize() * 100;
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//ActorSpawnParams.Instigator = this;
			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<ASingleLaserProjectile>(Tower->ProjectileClass, TowerLocation, TowerRotation, ActorSpawnParams);
		
			// try and play the sound if specified
			if (Tower->FireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Tower->FireSound, Tower->GetActorLocation());
			}

			return EBTNodeResult::Succeeded;
		}

		
	}
	
	return EBTNodeResult::Failed;
}
