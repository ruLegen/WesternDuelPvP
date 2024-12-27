// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WDGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API AWDGamePlayerState : public APlayerState
{
	GENERATED_BODY()
	

protected:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	uint32 Deaths;

	UPROPERTY(Replicated)
	uint32 Kills;

public:

	UFUNCTION(Server, reliable)
	void AddKill(int Num);
	
	UFUNCTION(Server, reliable)
	void AddDeath(int Num);

	UFUNCTION(Server, reliable)
	void ResetScore();

	UFUNCTION(Server, reliable)
	void ResetBonus();

	uint32 GetKills() { return Kills; }
	uint32 GetDeaths() { return Deaths; }
	uint32 GetPoints() { return Kills*2 - Deaths; }

};
