// Fill out your copyright notice in the Description page of Project Settings.


#include "WDLobbyGameState.h"
#include "Net/UnrealNetwork.h"

void AWDLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDLobbyGameState, PlayerNames);
}

void AWDLobbyGameState::OnRep_PlayerNameUpdated()
{
	OnPlayerUpdate.Broadcast(PlayerNames);
}

void AWDLobbyGameState::AddPlayer_Implementation(const FString& Player)
{
	PlayerNames.Add(Player);
	OnRep_PlayerNameUpdated();
}

void AWDLobbyGameState::RemovePlayer_Implementation(const FString& Player)
{
	PlayerNames.Remove(Player);
	OnRep_PlayerNameUpdated();
}
