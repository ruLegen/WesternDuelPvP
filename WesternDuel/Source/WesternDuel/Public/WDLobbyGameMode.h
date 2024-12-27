// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WDLobbyGameMode.generated.h"

/**
 * 
 */
class AWDLobbyPlayerController;
UCLASS()
class WESTERNDUEL_API AWDLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TArray<AWDLobbyPlayerController*> Players;
	
	UPROPERTY()
	int32 MaxPlayer = 2;


public:
	AWDLobbyGameMode();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting);

	int32 GetNumOfConnectedPlayers() { return Players.Num(); }
	int32 GetMaxPlayers () { return MaxPlayer; }

};
