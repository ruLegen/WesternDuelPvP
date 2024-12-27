// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WDPickUpBase.generated.h"


class USoundCue;
class ACharacter;
class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class WESTERNDUEL_API AWDPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWDPickUpBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPickedUp_Implementation(ACharacter* Controller) {};

	UPROPERTY(ReplicatedUsing=OnStateChanged)
	bool bIsHidden = false;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* PickUpSound;

	

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(Server, Reliable)
	void Reset();

	UFUNCTION(NetMulticast, Reliable)
	void PlayPickUpSound(FVector Location);

	UFUNCTION(Server, Reliable)
	void OnPickedUp(ACharacter* Controller);

	UFUNCTION()
	void OnStateChanged();

	

};
