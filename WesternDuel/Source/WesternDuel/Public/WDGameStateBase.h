// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WDGameStateBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FGameStateEventSignature)

UCLASS()
class WESTERNDUEL_API AWDGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_RoundChanged)
	int32 CurrentRound = 0;

	UPROPERTY(ReplicatedUsing=OnRep_GameOver)
	bool bIsGameOver = false;

	UPROPERTY()
	float DelayOnEmpty = 4;	// Seconds; Delay for next round if All players has no bullets
	
	UPROPERTY()
	float DelayAfterDeath = 1.1245657564;	// Seconds; Delay before EndRound

	UPROPERTY()
	float DelayOnEndRound = 2;	// Seconds; How many seconds between EndRound and RestartRound

	UPROPERTY(replicated)
	float DelayOnStartRound = FMath::RandRange(5,10);	// Seconds; Delay before can shoot
	

	
	UPROPERTY(Replicated)
	int32 EmptyPlayersCount= 0;		// Count how many players has no ammo

	FTimerHandle Timer;
	
	UFUNCTION()
	void OnRep_RoundChanged();
	
	UFUNCTION()
	void OnRep_GameOver();


public:
	FGameStateEventSignature OnRoundStart;		// Called when players can shoot
	FGameStateEventSignature OnRoundEnd;		// Called when Someone died
	FGameStateEventSignature OnRoundChanged;	// Called After End and before Start
	FGameStateEventSignature OnGameOver;
	
	int32 GetCurrentRound() { return CurrentRound; };
	
	UFUNCTION(Server, Reliable)
	void NextRound();
	
	UFUNCTION(NetMulticast, Reliable)
	void OnChangeRound();

	UFUNCTION(NetMulticast, Reliable)
	void StartRound();

	UFUNCTION(NetMulticast, Reliable)
	void EndRound();

	UFUNCTION(Server, Reliable)
	void ResetMatch();

	UFUNCTION(Server, Reliable)
	void NotifyEmpty();

	UFUNCTION(Server, Reliable)
	void UpdateRoundStart();

};
