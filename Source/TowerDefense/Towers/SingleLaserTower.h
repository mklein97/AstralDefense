// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TowerInterface.h"
#include "../AstralDefensePlayerController.h"
#include "../Decals/AstralDefenseDecalComponent.h"
#include "SingleLaserTower.generated.h"

UCLASS()
class TOWERDEFENSE_API ASingleLaserTower : public AActor, public ITowerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleLaserTower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Properties")
	FTowerObjectData TowerObjectData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAstralDefenseDecalComponent* UnableDecalComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAstralDefenseDecalComponent* AttackRadiusDecalComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float testfloat;

	APlayerController* PlayerController;
	FVector MouseLocation;
	FVector MouseDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FTowerObjectData* GetDataStruct();

	//bool IsPlacing();
	void SetPlaced();
	void DisableAttackRadiusDecal();

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromHit, const FHitResult& Hit);

	UFUNCTION()
	void OffCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsCollidingWith(ITowerInterface &otherActor);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};