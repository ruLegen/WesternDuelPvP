// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDMainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/WDMainMenuWidget.h"
#include "UI/WDLobbySearchWidget.h"

void AWDMainMenuHUD::BeginPlay() {
	Super::BeginPlay();
	if (MenuWidgetClass) {
		MenuWidget = CreateWidget<UWDMainMenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->OnJoinRequest.AddUObject(this, &AWDMainMenuHUD::OnJoinRequest);
			MenuWidget->AddToViewport();
		}
	}
	if (LobbySearchWidgetClass) {
		LobbyWidget = CreateWidget<UWDLobbySearchWidget>(GetWorld(), LobbySearchWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			LobbyWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void AWDMainMenuHUD::OnJoinRequest() {
	if (LobbyWidget && MenuWidget) {
		MenuWidget->SetVisibility(ESlateVisibility::Hidden);
		LobbyWidget->SetVisibility(ESlateVisibility::Visible);
	}
}