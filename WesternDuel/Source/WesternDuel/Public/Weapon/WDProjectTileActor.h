// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WDProjectTileActor.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USoundCue;
UCLASS()
class WESTERNDUEL_API AWDProjectTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWDProjectTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, replicated, Category = "ProjectTile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectTile")
	USphereComponent* SphereCollider;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectTile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectTile")
	float BaseDamage = 40;

	UPROPERTY(EditDefaultsOnly)
	TMap<UPhysicalMaterial*, float> PhysicsDamageMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="VFX")
	TArray<UNiagaraSystem*> BloodFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* BodyHitSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FlyBySound;

	UPROPERTY(replicated)
	FVector ShotDirection;

	UPROPERTY()
	AController* ShotController;

	UPROPERTY()
	float LifeSpan=3;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32  OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	AController* GetInsigatorController() { return ShotController; }
	void SetShotDirection(FVector Vector);
	void SetShotController(AController* Controller);
	void SetLifeTime(float Time);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void SpawnBloodFX(USceneComponent* AttachToComponent, FName AttachPointName,  FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void PlaySound(USoundCue* Sound,FVector Location);
};
