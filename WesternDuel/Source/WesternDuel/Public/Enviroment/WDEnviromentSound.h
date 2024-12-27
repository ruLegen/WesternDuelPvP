// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WDEnviromentSound.generated.h"

class USceneComponent;
class USoundCue;

USTRUCT(BlueprintType)
struct FSoundLocation
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector Location", Meta = (MakeEditWidget = true))
	FVector Location;

	UPROPERTY(EditAnywhere)
	USoundCue* Sound;

};
UCLASS()
class WESTERNDUEL_API AWDEnviromentSound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWDEnviromentSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TArray<FSoundLocation> SoundArray;


public:
	UFUNCTION(NetMulticast,Reliable)
	void OnRoundStart();

};
