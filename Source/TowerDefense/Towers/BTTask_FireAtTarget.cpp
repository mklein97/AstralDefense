// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FireAtTarget.h"

#include "AITowerController.h"
#include "AILineTowerController.h"

#include "Kismet/GameplayStatics.h"
#include "../Projectiles/SingleLaserProjectile.h"
#include "OneMissileTower.h"
#include "InLineTower.h"
/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"

#include "DrawDebugHelpers.h"

EBTNodeResult::Type UBTTask_FireAtTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	AAITowerController* MissileController = Cast<AAITowerController>(OwnerComp.GetAIOwner());
	AAILineTowerController* LineController = Cast<AAILineTowerController>(OwnerComp.GetAIOwner());
	if (MissileController == nullptr && LineController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	if (LineController)
	{
		APawn* Target = LineController->GetTargetEnemy();
		AInLineTower* LineTower = LineController->GetSelfActor();
		UE_LOG(LogTemp, Warning, TEXT("Got Controller"));

		if (Target)
		{
			FVector TLocation = LineController->GetTargetLocation(Target);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Attempting to Fire Laser"));
			UE_LOG(LogTemp, Warning, TEXT("Got Target"));


			// try and fire a projectile
			if (LineTower && LineTower->TowerObjectData.bPlaced &&LineTower->Particle)
			{

				UE_LOG(LogTemp, Warning, TEXT("Got Tower"));

				//FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
				//FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
				FRotator SpawnRotation = LineTower->GetActorRotation();

				DrawDebugLine(
					GetWorld(),
					LineTower->GetActorLocation() + FVector(0, 0, 200),
					LineTower->GetActorLocation() + FVector(0, 0, 200) + SpawnRotation.Vector().GetSafeNormal() * 200,
					FColor::Red,
					false,
					1.0f);

				FVector SpawnLocation = LineTower->GetActorLocation() + FVector(0, 0, 50) + SpawnRotation.Vector().GetSafeNormal() * 100;
				/*DrawDebugSphere(
					GetWorld(),
					SpawnLocation,
					50,
					12,
					FColor::Blue,
					false,
					3.0f);
					*/
					//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				//ActorSpawnParams.Instigator = this;
				/// spawn the projectile at the muzzle
				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LineTower->Particle, SpawnLocation, SpawnRotation, false);				
				UGameplayStatics::SpawnEmitterAttached(LineTower->Particle, LineTower->MeshComp, "TempParticle", SpawnLocation, SpawnRotation, EAttachLocation::KeepWorldPosition, false);
				//auto Projectile = Cast<UParticleSystem>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), LineTower->Particle, FTransform(SpawnRotation, SpawnLocation)));
				//GetWorld()->SpawnActor<ASingleLaserProjectile>(Tower->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				//if (Projectile != nullptr)
				//{
					//Projectile->Init(Target);
					UE_LOG(LogTemp, Warning, TEXT("In Fire At Target"));

				//	UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SpawnRotation, SpawnLocation));
				//}
				/// try and play the sound if specified
				if (LineTower->FireSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, LineTower->FireSound, LineTower->GetActorLocation());
				}

				return EBTNodeResult::Succeeded;
			}


		}

		return EBTNodeResult::Failed;
	}



	if (MissileController)
	{
		APawn* Target = MissileController->GetTargetEnemy();
		AOneMissileTower* MissileTower = MissileController->GetSelfActor();
		if (Target)
		{
			FVector TLocation = MissileController->GetTargetLocation(Target);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Attempting to Fire Laser"));


			// try and fire a projectile
			if (Target && MissileTower && MissileTower->TowerObjectData.bPlaced && MissileTower->ProjectileClass)
			{


				//FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
				//FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
				FRotator SpawnRotation = MissileTower->GetActorRotation();

				DrawDebugLine(
					GetWorld(),
					MissileTower->GetActorLocation() + FVector(0, 0, 200),
					MissileTower->GetActorLocation() + FVector(0, 0, 200) + SpawnRotation.Vector().GetSafeNormal() * 200,
					FColor::Red,
					false,
					1.0f);

				FVector SpawnLocation = MissileTower->GetActorLocation() + FVector(0, 0, 380) + SpawnRotation.Vector().GetSafeNormal() * 200;
				/*DrawDebugSphere(
					GetWorld(),
					SpawnLocation,
					50,
					12,
					FColor::Blue,
					false,
					3.0f);
					*/
					//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				//ActorSpawnParams.Instigator = this;
				// spawn the projectile at the muzzle
				auto Projectile = Cast<ASingleLaserProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), MissileTower->ProjectileClass, FTransform(SpawnRotation, SpawnLocation)));
				//GetWorld()->SpawnActor<ASingleLaserProjectile>(Tower->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if (Projectile != nullptr)
				{
					Projectile->Init(MissileController);
					UE_LOG(LogTemp, Warning, TEXT("In Fire At Target"));

					UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SpawnRotation, SpawnLocation));
				}
				// try and play the sound if specified
				if (MissileTower->FireSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, MissileTower->FireSound, MissileTower->GetActorLocation());
				}

				return EBTNodeResult::Succeeded;
			}


		}

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
