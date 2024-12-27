// Fill out your copyright notice in the Description page of Project Settings.


#include "WDLobbyPlayerController.h"
#include "WDLobbyGameState.h"
#include "Gameframework/PlayerState.h"
#include "UI/WDLobbyHUD.h"
#include "WDGameInstance.h"
#include "WDLobbyPlayerState.h"

void AWDLobbyPlayerController::BeginPlay() {
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

};
void AWDLobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
};


void AWDLobbyPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
	{
		ViewportClient->RemoveAllViewportWidgets();
	}
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
}