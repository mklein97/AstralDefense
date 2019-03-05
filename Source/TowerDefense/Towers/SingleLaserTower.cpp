// Fill out your copyright notice in the Description page of Project Settings.
#include "SingleLaserTower.h"
#include "DrawDebugHelpers.h"
#include "../Projectiles/SingleLaserProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

// Sets default values
ASingleLaserTower::ASingleLaserTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//TowerObjectData.MeshComp->bEditableWhenInherited = true;
	//TowerObjectData.AttackRadiusDecalComp->bEditableWhenInherited = true;

	TowerObjectData.AttackRadius = 1000;
	TowerObjectData.LaserFireRate = 2;

	////////////////
	UnableDecalComp = CreateDefaultSubobject<UAstralDefenseDecalComponent>(TEXT("Unable Decal Comp"));
	AttackRadiusDecalComp = CreateDefaultSubobject<UAstralDefenseDecalComponent>(TEXT("Attack  Decal Comp"));

	ConstructorHelpers::FObjectFinder<UMaterial>PlacementObj(TEXT("Material'/Game/TopDownCPP/Materials/M_Tower_Placement.M_Tower_Placement'"));
	if (PlacementObj.Succeeded())
	{
		UMaterial *PlacementMat = PlacementObj.Object;
		TowerObjectData.PlacementMatInst = (UMaterialInstanceDynamic*)PlacementMat;
	}

	AttackRadiusDecalComp->SetDecalMaterial(TowerObjectData.PlacementMatInst);
	AttackRadiusDecalComp->SetActive(true);


	ConstructorHelpers::FObjectFinder<UMaterial>UnableObj(TEXT("Material'/Game/TopDownCPP/Materials/M_Unable_Placement.M_Unable_Placement'"));
	if (UnableObj.Succeeded())
	{
		UMaterial *UnableMat = UnableObj.Object;
		TowerObjectData.UnableMatInst = (UMaterialInstanceDynamic*)UnableMat;
	}
	////////////////////////
	UnableDecalComp->SetDecalMaterial(TowerObjectData.UnableMatInst);


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
	AttackRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Radius Comp"));

	CollisionComp->SetRelativeTransform(
		FTransform(
			FRotator(0, 0, 0),
			FVector(0, 0, 50),
			FVector(1.0, 1.0, 1.0)));


	FTransform DecalTransform = FTransform(
		FRotator(90.0, 0.0, 0.0),	// Rotation
		FVector(0.0, 0.0, 0.0),		// Location
		FVector(1.0, 1.0, 1.0));	// Scale
	AttackRadiusDecalComp->SetRelativeTransform(DecalTransform);
	UnableDecalComp->SetRelativeTransform(DecalTransform);

	AttackRadiusDecalComp->DecalSize = FVector(5, TowerObjectData.AttackRadius, TowerObjectData.AttackRadius);
	UnableDecalComp->DecalSize = FVector(5, TowerObjectData.AttackRadius, TowerObjectData.AttackRadius);



	MeshComp->SetupAttachment(RootComponent);
	UnableDecalComp->SetupAttachment(MeshComp);
	AttackRadiusDecalComp->SetupAttachment(MeshComp);

	CollisionComp->SetupAttachment(MeshComp);
	AttackRadiusComp->SetupAttachment(MeshComp);


	RootComponent = MeshComp;

	TowerObjectData.Cost = 50;



}

// Called when the game starts or when spawned
void ASingleLaserTower::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
	{
		Cast<AAstralDefensePlayerController>(PlayerController)->SetPlacingTower(this);
	}



	AttackRadiusDecalComp->UpdateBounds();
	AttackRadiusDecalComp->SetDecalMaterial(TowerObjectData.PlacementMatInst);
	AttackRadiusDecalComp->RegisterComponent();
	AttackRadiusDecalComp->SetActive(true);

	UnableDecalComp->SetDecalMaterial(TowerObjectData.UnableMatInst);
	UnableDecalComp->RegisterComponent();
	UnableDecalComp->SetActive(false);

	TowerObjectData.DecalBounds = AttackRadiusDecalComp->CalcBounds(FTransform());//AttackRadiusDecalComp->GetRelativeTransform());

	//UE_LOG(LogTemp, Warning, TEXT("Sphere Radius: %f"), TowerObjectData.MeshBounds.SphereRadius);
	//UE_LOG(LogTemp, Warning, TEXT("GETSPHERE Center: %s"), *TowerObjectData.MeshBounds.GetSphere().Center.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("GETSPHERE Radius: %f"), TowerObjectData.MeshBounds.GetSphere().W);

	TowerObjectData.MeshBounds = MeshComp->CalcBounds(FTransform());
	//UE_LOG(LogTemp, Warning, TEXT("MeshBounds Box Extent: %s"), *TowerObjectData.MeshBounds.BoxExtent.ToString());

	CollisionComp->SetSphereRadius((TowerObjectData.MeshBounds.BoxExtent.X + TowerObjectData.MeshBounds.BoxExtent.Y) / 2);
	AttackRadiusComp->SetSphereRadius(TowerObjectData.AttackRadius);

	TowerObjectData.CollisionBounds = CollisionComp->CalcBounds(FTransform(
		FRotator(0, 0, 0),
		FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 50),
		FVector(1, 1, 1)));
	//UE_LOG(LogTemp, Warning, TEXT("GETSPHERE Radius: %f"), TowerObjectData.CollisionBounds.GetSphere().W);

	CollisionComp->bHiddenInGame = false;
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASingleLaserTower::OnCollision);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ASingleLaserTower::OffCollision);

	AttackRadiusComp->bHiddenInGame = false;
	AttackRadiusComp->SetGenerateOverlapEvents(true);
	AttackRadiusComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AttackRadiusComp->OnComponentBeginOverlap.AddDynamic(this, &ASingleLaserTower::OnSeen);
	AttackRadiusComp->OnComponentEndOverlap.AddDynamic(this, &ASingleLaserTower::OffSeen);

	//UE_LOG(LogTemp, Warning, TEXT("This Sphere: %f"), this->TowerObjectData.CollisionBounds.GetSphere().W);
	//UE_LOG(LogTemp, Warning, TEXT("This Sphere: %f"), CollisionComp->GetScaledSphereRadius());



	//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Radius: %f"), CollisionComp->CalcBounds(FTransform()).SphereRadius);
	//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Center: %s"), *CollisionComp->CalcBounds(FTransform()).GetSphere().Center.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Sphere Radius: %f"), CollisionComp->CalcBounds(FTransform()).GetSphere().W);

	TowerObjectData.ActorLocation = this->GetActorLocation();
	
	TowerObjectData.LaserFireRate = 2;
	TowerObjectData.Reloading = false;
}

FTowerObjectData * ASingleLaserTower::GetDataStruct()
{
	// TODO: insert return statement here

	return &TowerObjectData;
}

/*
bool ASingleLaserTower::IsPlacing()
{
	if (TowerObjectData.bPlacing)
	{
		return true;
	}
	else
	{
		return false;
	}
}*/

void ASingleLaserTower::SetPlaced()
{
	ITowerInterface::SetPlaced();
	TowerObjectData.ActorLocation = this->GetActorLocation();
	TowerObjectData.CollisionBounds = CollisionComp->CalcBounds(FTransform(
		FRotator(0, 0, 0),
		FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 50),
		FVector(1, 1, 1)));
	DisableAttackRadiusDecal();
	//this->TowerObjectData.bPlaced = true;
	//DisableAttackRadiusDecal();
	//UE_LOG(LogTemp, Warning, TEXT("COMPARE TOWER: %s"), *this->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Sphere in Placed: %f"), this->TowerObjectData.CollisionBounds.GetSphere().W);

}

void ASingleLaserTower::DisableAttackRadiusDecal()
{
	//UE_LOG(LogTemp, Warning, TEXT("In Disable"));
	AttackRadiusDecalComp->UnregisterComponent();
	UnableDecalComp->UnregisterComponent();
}

void ASingleLaserTower::OnCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult & Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We got a collision."));

	if (this->TowerObjectData.bPlacing)
	{
		if ((OtherActor != NULL))// && (OtherActor != this) && (OtherComp != NULL))
		{
			//ASingleLaserTower * OtherTower = Cast<ASingleLaserTower>(OtherActor);

			AttackRadiusDecalComp->UnregisterComponent();
			AttackRadiusDecalComp->SetActive(false);
			UnableDecalComp->RegisterComponent();
			UnableDecalComp->SetActive(true);
			UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to Red!"));


			TowerObjectData.bCollidesToggle = false;
			TowerObjectData.bNotCollidesToggle = true;
		}

	}
}

void ASingleLaserTower::OffCollision(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (this->TowerObjectData.bPlacing)
	{
		UnableDecalComp->UnregisterComponent();
		UnableDecalComp->SetActive(false);
		AttackRadiusDecalComp->RegisterComponent();
		AttackRadiusDecalComp->SetActive(true);
		UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to GREEN!"));


		TowerObjectData.bNotCollidesToggle = false;
		TowerObjectData.bCollidesToggle = true;
	}
}

void ASingleLaserTower::OnSeen(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult & Hit)
{
	if (OtherActor->IsA(ASingleLaserProjectile::StaticClass()))
	{

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Seen a Pawn"));

		if (this->TowerObjectData.CurrentTarget == nullptr)
		{

			UE_LOG(LogTemp, Warning, TEXT("Got a target!"));
			this->TowerObjectData.CurrentTarget = Cast<APawn>(OtherActor);


		}
		else
		{

		}
		this->TowerObjectData.AllTargets.Add(Cast<APawn>(OtherActor));
	}

}

void ASingleLaserTower::OffSeen(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor->IsA(ASingleLaserProjectile::StaticClass()))
	{

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Pawn is leaving"));

		this->TowerObjectData.AllTargets.Remove(TowerObjectData.CurrentTarget);
		if (TowerObjectData.AllTargets.Num() != 0)
		{
			this->TowerObjectData.CurrentTarget = TowerObjectData.AllTargets[0];
		}
		else
		{
			this->TowerObjectData.CurrentTarget = nullptr;
		}
	}
}

void ASingleLaserTower::ResetFire()
{
	this->TowerObjectData.Reloading = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_FireRate);
}

void ASingleLaserTower::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Attempting to Fire Laser"));

	// try and fire a projectile
	if (ProjectileClass)
	{
		//FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		//FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");
		FRotator TowerRotation = this->GetActorRotation();
		FVector TowerLocation = this->GetActorLocation() + TowerRotation.Vector().Normalize() *100;
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		//ActorSpawnParams.Instigator = this;
		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<ASingleLaserProjectile>(ProjectileClass, TowerLocation, TowerRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

bool ASingleLaserTower::IsCollidingWith(ITowerInterface &otherActor)
{
	FTowerObjectData* tempTOD = otherActor.GetDataStruct();
	/*
	UE_LOG(LogTemp, Warning, TEXT("|"));

	UE_LOG(LogTemp, Warning, TEXT("This Sphere in Colliding: %s"), *this->TowerObjectData.CollisionBounds.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Other Actor Sphere: %s"), *tempTOD->CollisionBounds.ToString());
	UE_LOG(LogTemp, Warning, TEXT("|"));
	*/
	/*
	if (FBoxSphereBounds::SpheresIntersect(this->TowerObjectData.CollisionBounds, tempTOD->CollisionBounds))
	{

		if (TowerObjectData.bCollidesToggle) {
			AttackRadiusDecalComp->UnregisterComponent();
			AttackRadiusDecalComp->SetActive(false);
			UnableDecalComp->RegisterComponent();
			UnableDecalComp->SetActive(true);
			UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to Red!"));


			TowerObjectData.bCollidesToggle = false;
			TowerObjectData.bNotCollidesToggle = true;
		}

		return true;
	}
	else
	{
		if (TowerObjectData.bNotCollidesToggle)
		{
			UnableDecalComp->UnregisterComponent();
			UnableDecalComp->SetActive(false);
			AttackRadiusDecalComp->RegisterComponent();
			AttackRadiusDecalComp->SetActive(true);
			UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to GREEN!"));


			TowerObjectData.bNotCollidesToggle = false;
			TowerObjectData.bCollidesToggle = true;
		}

		return false;
	}
	*/
	return false;
}

// Called every frame
void ASingleLaserTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (PlayerController != nullptr)
	{

		if (!TowerObjectData.bPlaced)
		{
			FHitResult TraceHitResult;
			PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			this->SetActorLocation(TraceHitResult.Location);
			this->SetActorRotation(this->GetActorRotation());
			//UE_LOG(LogTemp, Warning, TEXT("CursorLocation: %s"), *TraceHitResult.Location.ToString());

			/*
			if (CollisionComp)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Center: %s"), *CollisionComp->CalcBounds(FTransform()).GetSphere().Center.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Sphere Radius: %f"), CollisionComp->CalcBounds(FTransform()).GetSphere().W);
				DrawDebugSphere(
					GetWorld(),
					TraceHitResult.Location,
					CollisionComp->GetScaledSphereRadius(),
					12,
					FColor::Red,
					false,
					1.0f);
			}
			*/
		}
		else
		{
			if (TowerObjectData.CurrentTarget != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Looking at target!"));

				DrawDebugSphere(
					GetWorld(),
					TowerObjectData.CurrentTarget->GetActorLocation(),
					50,
					12,
					FColor::Emerald,
					false,
					1.0f);
				
				FVector Direction = TowerObjectData.CurrentTarget->GetActorLocation() - this->GetActorLocation();
				Direction.Normalize();

				FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
				NewLookAt.Pitch = 0.0f;
				NewLookAt.Roll = 0.0f;
				SetActorRotation(NewLookAt);
				if (!this->TowerObjectData.Reloading)
				{
					UE_LOG(LogTemp, Warning, TEXT("Shoot!!"));
					UE_LOG(LogTemp, Warning, TEXT("Fire Rate: %f"), TowerObjectData.LaserFireRate);
					Fire();
					GetWorldTimerManager().SetTimer(TimerHandle_FireRate, this, &ASingleLaserTower::ResetFire, 2.0f);

					this->TowerObjectData.Reloading = true;
				}

				//UE_LOG(LogTemp, Warning, TEXT("Current Time is %f"), GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_FireRate));
				
			}
		}
		/*
		if (CollisionComp)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Center: %s"), *CollisionComp->CalcBounds(FTransform()).GetSphere().Center.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("Collision Sphere Sphere Radius: %f"), CollisionComp->CalcBounds(FTransform()).GetSphere().W);
			DrawDebugSphere(
				GetWorld(),
				TowerObjectData.ActorLocation,
				CollisionComp->GetScaledSphereRadius(),
				12,
				FColor::Red,
				false,
				1.0f);
		}
		*/
	}

	//UE_LOG(LogTemp, Warning, TEXT("Relative Location, %d, %d, %d"), AttackRadiusDecalComp->getlocation );
}
