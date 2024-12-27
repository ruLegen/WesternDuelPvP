// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "WDPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API AWDPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	AWDPlayerStart(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, Category = "Round")
	int32 RoundNumber= 0;							// Indicate when (in which round) this Player Start will be using by GameMode

public:
	UPROPERTY(EditAnywhere, Replicated, Category = "Round", Transient)
	bool bIsOccupied = false;						// Indicate whether this PlayerStart occupied by some PlayerControlelr or not; It simplify PlayerStart selection by GameMode

	int32 GetRoundNumber() { return RoundNumber; }
	void SetRoundNumber(int32 Round) { RoundNumber = Round; };
	bool IsOccupied() { return bIsOccupied; };
	
	UFUNCTION(Server, Reliable)
	void SetOccupition(bool IsOccupied);

};
