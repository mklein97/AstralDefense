// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleLaserProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASingleLaserProjectile::ASingleLaserProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASingleLaserProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	//ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
}

// Called when the game starts or when spawned
void ASingleLaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASingleLaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASingleLaserProjectile::Init(APawn* targetEnemy)
{
	TargetEnemy = targetEnemy;
	UE_LOG(LogTemp, Warning, TEXT("Target : %s"), *TargetEnemy->GetName());

	HomingMissile(TargetEnemy);
}

void ASingleLaserProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit!"));

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	}

	//MakeNoise(1.0f, Instigator);
	Destroy();
}

void ASingleLaserProjectile::HomingMissile(APawn * Target)
{
	TArray<UStaticMeshComponent*> StaticComps;
	AActor* TargetActor = Cast<AActor>(Target);
	TargetActor->GetComponents<UStaticMeshComponent>(StaticComps);

	ProjectileMovement->HomingTargetComponent = StaticComps[0];
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 20000;
}
