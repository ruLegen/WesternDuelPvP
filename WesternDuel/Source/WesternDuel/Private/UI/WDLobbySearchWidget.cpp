// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDLobbySearchWidget.h"
#include "UI/WDLobbyServerRowWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WDGameInstance.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"

#include "Components/ScrollBox.h"
#include "Components/Button.h"


void UWDLobbySearchWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto GameInstace = GetGameInstance<UWDGameInstance>();
	if (GameInstace)
	{
		GameInstace->OnSessionFoundEvent.AddUObject(this, &UWDLobbySearchWidget::OnRefresh);
	};

	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &UWDLobbySearchWidget::Refresh);
	}
	Refresh();
}

void UWDLobbySearchWidget::Refresh()
{

	auto GameInstace = GetGameInstance<UWDGameInstance>();
	if (!GameInstace) return;

	GameInstace->RequestSearchSession();
}

void UWDLobbySearchWidget::OnRefresh(TArray<FOnlineSessionSearchResult>& Servers)
{
	UpdateServers(Servers);
}

void UWDLobbySearchWidget::UpdateServers(TArray<FOnlineSessionSearchResult>& Servers)
{
	ServerListScrollBox->ClearChildren();
	int32 index = 0;
	for (auto Server : Servers) {

		auto ServerRow = CreateWidget<UWDLobbyServerRowWidget>(GetWorld(), ServerRowClass);
		if (!ServerRow) continue;
		ServerRow->SetServerName(Server.GetSessionIdStr());
		ServerRow->SetIndex(index);
		ServerRow->OnClicked.AddUObject(this, &UWDLobbySearchWidget::OnRowClicked);
		ServerListScrollBox->AddChild(ServerRow);
		index++;
	}
}

void UWDLobbySearchWidget::OnRowClicked(int32 Id)
{

	auto GameInstace = GetGameInstance<UWDGameInstance>();
	if (!GameInstace) return;

	GameInstace->Join(Id);
}
