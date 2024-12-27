// Fill out your copyright notice in the Description page of Project Settings.


#include "WDLobbyGameMode.h"
#include "WDLobbyPlayerController.h"
#include "WDLobbyGameState.h"
#include "UI/WDLobbyHUD.h"
#include "Character/WDMainMenuPlayerController.h"
#include "WDLobbyPlayerState.h"
#include "WDLobbyPlayerController.h"


AWDLobbyGameMode::AWDLobbyGameMode() 
{
	HUDClass = AWDLobbyHUD::StaticClass();
	PlayerControllerClass = AWDLobbyPlayerController::StaticClass();
	GameStateClass = AWDLobbyGameState::StaticClass();
	PlayerStateClass = AWDLobbyPlayerState::StaticClass();
	bUseSeamlessTravel = true;
}

void AWDLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AWDLobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (Players.Num() >= MaxPlayer) {
		ErrorMessage = "No more slots in this lobby";
		return nullptr;
	}
	else
		return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}
void AWDLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	AWDLobbyPlayerController* Controller = Cast<AWDLobbyPlayerController>(NewPlayer);
	if (!Controller) return;
	Players.Add(Controller);

	auto LobbyGameState = GetWorld()->GetGameState<AWDLobbyGameState>();
	if (!LobbyGameState) return;

	LobbyGameState->AddPlayer(Controller->GetName());
	//UE_LOG(LogTemp, Error, TEXT("Joined %s"), *NewPlayer->GetName());

}

void AWDLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AWDLobbyPlayerController* Controller = Cast<AWDLobbyPlayerController>(Exiting);
	if (!Controller) return;
	Players.Remove(Controller);

}
