// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleLaserProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "../Towers/AITowerController.h"

#include "DrawDebugHelpers.h"
// Sets default values
ASingleLaserProjectile::ASingleLaserProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->bGenerateOverlapEventsDuringLevelStreaming = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->SetGenerateOverlapEvents(true);
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	//ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
}

// Called when the game starts or when spawned
void ASingleLaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASingleLaserProjectile::OnHit);	// set up a notification for when this component hits something blocking

}

// Called every frame
void ASingleLaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHoming)
	{
		if (!TargetEnemy)
		{
			// Current Target Killed Returns Next Target
			TargetEnemy = MissileController->TargetWasKilled();
			if (!TargetEnemy)
			{
				bHoming = false;
			}
			else
			{
				HomingMissile(TargetEnemy);
			}
		}
	}
}

void ASingleLaserProjectile::Init(AAITowerController* missileController)
{
	MissileController = missileController;
	
	TargetEnemy = MissileController->GetTargetEnemy();
	UE_LOG(LogTemp, Warning, TEXT("Target : %s"), *TargetEnemy->GetName());

	HomingMissile(TargetEnemy);
}

void ASingleLaserProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult& Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit! : %s"), *OtherActor->GetName());
		bHoming = false;
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	}
	/*
	DrawDebugSphere(
		GetWorld(),
		Hit.Location + FVector(0,0,100),
		100,
		12,
		FColor::Red,
		false,
		3.0f);
		*/
	//MakeNoise(1.0f, Instigator);
	/*
	if (OtherActor == TargetEnemy)
	{
		Destroy();
	}
	*/
}

void ASingleLaserProjectile::HomingMissile(APawn * Target)
{
	TArray<UStaticMeshComponent*> StaticComps;
	AActor* TargetActor = Cast<AActor>(Target);
	TargetActor->GetComponents<UStaticMeshComponent>(StaticComps);
	bHoming = true;

	ProjectileMovement->HomingTargetComponent = StaticComps[0];
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 30000;
}
