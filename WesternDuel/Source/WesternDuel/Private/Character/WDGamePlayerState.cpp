// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WDGamePlayerState.h"
#include "Net/UnrealNetwork.h"

void AWDGamePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDGamePlayerState, Kills);
	DOREPLIFETIME(AWDGamePlayerState, Deaths);
}

void AWDGamePlayerState::AddKill_Implementation(int Num)
{
	Kills += Num;
}

void AWDGamePlayerState::AddDeath_Implementation(int Num)
{
	Deaths += Num;
}

void AWDGamePlayerState::ResetScore_Implementation()
{
	Kills = 0;
	Deaths = 0;
}

void AWDGamePlayerState::ResetBonus_Implementation()
{
	// TODO ResetBonus
}
