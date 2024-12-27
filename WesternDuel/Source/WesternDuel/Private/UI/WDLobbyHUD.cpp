// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDLobbyHUD.h"
#include "Blueprint/UserWidget.h"
#include "WDLobbyGameState.h"
#include "UI/WDLobbyWidget.h"
#include "WDLobbyGameMode.h"
void AWDLobbyHUD::BeginPlay() {
	Super::BeginPlay();
	auto LobbyState = GetWorld()->GetGameState<AWDLobbyGameState>();
	if (!LobbyState) return;
	
	LobbyState->OnPlayerUpdate.AddUObject(this, &AWDLobbyHUD::OnPlayersUpdate);
	LobbyWidget = Cast<UWDLobbyWidget>(CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass));
	if (!LobbyWidget) return;

	LobbyWidget->AddToViewport();

	if (GetLocalRole() == ROLE_Authority) {
		TArray<FString> NameArray;

		NameArray.Add(GetOwningPlayerController()->GetName());
		LobbyWidget->UpdatePlayers(NameArray);
	}
}

void AWDLobbyHUD::OnPlayersUpdate(TArray<FString> Players)
{
	if (!LobbyWidget) return;
	LobbyWidget->UpdatePlayers(Players);

	if (GetLocalRole() == ROLE_Authority) {
		auto GameMode = GetWorld()->GetAuthGameMode<AWDLobbyGameMode>();
		if (!GameMode) return;

		if (GameMode->GetNumOfConnectedPlayers() >= GameMode->GetMaxPlayers()) 
		{
			LobbyWidget->SetButtonVisibility(true);
		}
		else 
		{
			LobbyWidget->SetButtonVisibility(false);
		}
	}
}
