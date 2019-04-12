// Fill out your copyright notice in the Description page of Project Settings.

#include "InLineTower.h"
#include "AILineTowerController.h"
#include "STowerState.h"

#include "../Projectiles/SingleLaserProjectile.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/Classes/Components/AudioComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"


// Sets default values
AInLineTower::AInLineTower(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/* Note: We assign the Controller class in the Blueprint extension of this class
	Because the zombie AIController is a blueprint in content and it's better to avoid content references in code.  */


	TowerObjectData.AttackRadius = TowerObjectData.PlacementRadius * 50;
	TowerObjectData.PlacementRadius = 24;




	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);
	////////////////////////



	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MaterialPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Material"));
	MaterialPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MaterialPlane->SetRelativeLocation(FVector(0, 0, 1));



	TowerObjectData.LaserFireRate = 2;

	////////////////


	ConstructorHelpers::FObjectFinder<UMaterialInstance>PlacementObj(TEXT("MaterialInstanceConstant'/Game/Assets/Towers/Materials/M_TowerPlacing.M_TowerPlacing'"));
	if (PlacementObj.Succeeded())
	{
		PlacementMat = PlacementObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance>UnableObj(TEXT("MaterialInstanceConstant'/Game/Assets/Towers/Materials/M_TowerRange.M_TowerRange'"));
	if (UnableObj.Succeeded())
	{
		UnableMat = UnableObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance>RangeObj(TEXT("MaterialInstanceConstant'/Game/Assets/Towers/Materials/M_TowerUnable.M_TowerUnable'"));
	if (RangeObj.Succeeded())
	{
		AttackRadMat = RangeObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneObj(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (PlaneObj.Succeeded())
	{
		MaterialPlane->SetStaticMesh(PlaneObj.Object);
	}
	////////////////////////
	MaterialPlane->SetMaterial(0, TowerObjectData.PlacementMatInst);
	MaterialPlane->SetRelativeScale3D(FVector(TowerObjectData.PlacementRadius, TowerObjectData.PlacementRadius, 1.0));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));

	CollisionComp->SetRelativeTransform(
		FTransform(
			FRotator(0, 0, 0),
			FVector(0, 0, 50),
			FVector(1.0, 1.0, 1.0)));


	FTransform DecalTransform = FTransform(
		FRotator(90.0, 0.0, 0.0),	// Rotation
		FVector(0.0, 0.0, 0.0),		// Location
		FVector(1.0, 1.0, 1.0));	// Scale




	MeshComp->SetupAttachment(RootComponent);

	MaterialPlane->SetupAttachment(MeshComp);

	CollisionComp->SetupAttachment(MeshComp);


	RootComponent = MeshComp;

	TowerObjectData.Cost = 50;


	//Health = 100;
	MissileDamage = 24.0f;
	MissileStrikeCooldown = 1.0f;
	//SprintingSpeedModifier = 3.0f;

	/* By default we will not let the AI patrol, we can override this value per-instance. */
	TowerObjectData.TowerType = ETowerBehaviorType::Inactive;
	TowerObjectData.SenseTimeOut = 2.5f;

	/* Note: Visual Setup is done in the AI/ZombieCharacter Blueprint file */

	TowerObjectData.AILineController = Cast<AAILineTowerController>(GetController());
	TowerObjectData.CurrentTarget = nullptr;
	this->SetActorEnableCollision(false);

}

// Called when the game starts or when spawned
void AInLineTower::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
	{
		Cast<AAstralDefensePlayerController>(PlayerController)->SetPlacingTower(Cast<ITowerInterface>(this));
	}
	TowerObjectData.PlacementMatInst = UMaterialInstanceDynamic::Create(PlacementMat, this);

	MaterialPlane->SetMaterial(0, TowerObjectData.PlacementMatInst);
	MaterialPlane->SetRelativeScale3D(FVector(TowerObjectData.PlacementRadius, TowerObjectData.PlacementRadius, 1.0));
	MaterialPlane->SetRelativeLocation(FVector(0, 0, 1));

	TowerObjectData.MeshBounds = MeshComp->CalcBounds(FTransform());

	CollisionComp->SetSphereRadius((TowerObjectData.MeshBounds.BoxExtent.X + TowerObjectData.MeshBounds.BoxExtent.Y) / 2);

	TowerObjectData.CollisionBounds = CollisionComp->CalcBounds(FTransform(
		FRotator(0, 0, 0),
		FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 50),
		FVector(1, 1, 1)));

	CollisionComp->bHiddenInGame = false;
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AInLineTower::OnCollision);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AInLineTower::OffCollision);


	TowerObjectData.ActorLocation = this->GetActorLocation();

	TowerObjectData.LaserFireRate = 2;
	TowerObjectData.Reloading = false;

	//////////////////////////////////

	/* Assign a basic name to identify the bots in the HUD. */
	ASTowerState* TS = Cast<ASTowerState>(GetPlayerState());
	if (TS)
	{
		TS->SetPlayerName("Tower");
		TS->bIsABot = true;
	}


	TowerObjectData.AILineController = Cast<AAILineTowerController>(this->GetController());
	TowerObjectData.CurrentTarget = nullptr;

	this->SetActorEnableCollision(false);

}

// Called every frame
void AInLineTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TowerObjectData.bPlaced)
	{
		//APlayerController* Test = GetWorld()->GetFirstPlayerController();
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorFV = TraceHitResult.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		this->SetActorLocation(TraceHitResult.Location);
		this->SetActorRotation(this->GetActorRotation());
	}
	else
	{
		/*
		DrawDebugSphere(
			GetWorld(),
			this->GetActorLocation(),
			TowerObjectData.AttackRadius,
			12,
			FColor::Emerald,
			false,
			1.0f);
		*/
		if (!TowerObjectData.bSelected)
		{
			DisableAttackRadiusDecal();
		}

		TowerObjectData.AILineController = Cast<AAILineTowerController>(GetController());
		TowerObjectData.CurrentTarget = TowerObjectData.AILineController->GetTargetEnemy();
		if (TowerObjectData.AILineController && TowerObjectData.CurrentTarget)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceBetween);

			/*
			DrawDebugSphere(
				GetWorld(),
				TowerObjectData.CurrentTarget->GetActorLocation(),
				50,
				12,
				FColor::Emerald,
				false,
				1.0f);
				*/

			FVector Direction = TowerObjectData.CurrentTarget->GetActorLocation() - this->GetActorLocation();
			Direction.Normalize();

			FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
			NewLookAt.Pitch = 0.0f;
			NewLookAt.Roll = 0.0f;
			SetActorRotation(NewLookAt);


			//
			FVector DistanceBetVec = TowerObjectData.CurrentTarget->GetActorLocation() - this->GetActorLocation();
			float DistanceBetween = FMath::Abs(DistanceBetVec.Size());
			//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceBetween);

			/*
			if (DistanceBetween > TowerObjectData.AttackRadius)
			{
				bSensedTarget = false;
				AIController->SetTargetEnemy(nullptr);
			}
			*/
		}


		/* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
		/*if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut
			&& (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
		{
			bSensedTarget = false;
			// Reset
			AIController->SetTargetEnemy(nullptr);
		}
		*/

	}

}


FTowerObjectData * AInLineTower::GetDataStruct()
{
	return &TowerObjectData;
}

void AInLineTower::SetPlaced()
{
	ITowerInterface::SetPlaced();
	TowerObjectData.ActorLocation = this->GetActorLocation();
	TowerObjectData.CollisionBounds = CollisionComp->CalcBounds(FTransform(
		FRotator(0, 0, 0),
		FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 50),
		FVector(1, 1, 1)));
	DisableAttackRadiusDecal();
	this->SetActorEnableCollision(true);
}

int32 AInLineTower::TowerCost(int32 CurrentStarbucks)
{
	return ITowerInterface::TowerCost(CurrentStarbucks);
}

void AInLineTower::SetSelected()
{
	EnableAttackRadiusDecal();
	TowerObjectData.AttackRadMatInst = UMaterialInstanceDynamic::Create(AttackRadMat, this);
	TowerObjectData.bSelected = true;

	MaterialPlane->SetMaterial(0, TowerObjectData.AttackRadMatInst);
}

void AInLineTower::DisableAttackRadiusDecal()
{
	MaterialPlane->SetHiddenInGame(true);
}

void AInLineTower::EnableAttackRadiusDecal()
{
	MaterialPlane->SetHiddenInGame(false);
}


void AInLineTower::OnCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult & Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We got a collision."));

	if (this->TowerObjectData.bPlacing)
	{
		if ((OtherActor != NULL))// && (OtherActor != this) && (OtherComp != NULL))
		{
			//ASingleLaserTower * OtherTower = Cast<ASingleLaserTower>(OtherActor);
			EnableAttackRadiusDecal();
			TowerObjectData.UnableMatInst = UMaterialInstanceDynamic::Create(UnableMat, this);

			MaterialPlane->SetMaterial(0, TowerObjectData.UnableMatInst);

			UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to Red!"));


			TowerObjectData.bCollidesToggle = false;
			TowerObjectData.bNotCollidesToggle = true;
		}

	}
}

void AInLineTower::OffCollision(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (this->TowerObjectData.bPlacing)
	{
		EnableAttackRadiusDecal();
		TowerObjectData.PlacementMatInst = UMaterialInstanceDynamic::Create(PlacementMat, this);

		MaterialPlane->SetMaterial(0, TowerObjectData.PlacementMatInst);

		UE_LOG(LogTemp, Warning, TEXT("ChangingDecal to GREEN!"));


		TowerObjectData.bNotCollidesToggle = false;
		TowerObjectData.bCollidesToggle = true;
	}
}


void AInLineTower::SetTowerType(ETowerBehaviorType NewType)
{
	ITowerInterface::SetTowerType(NewType);

}

UAudioComponent * AInLineTower::PlayCharacterSound(USoundCue * CueToPlay)
{
	return nullptr;
}


