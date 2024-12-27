// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDLobbyServerRowWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWDLobbyServerRowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ServerButton)
	{
		ServerButton->OnClicked.AddDynamic(this, &UWDLobbyServerRowWidget::OnSelect);
	}
}

void UWDLobbyServerRowWidget::OnSelect()
{
	OnClicked.Broadcast(Index);
}

void UWDLobbyServerRowWidget::SetServerName(FString Name)
{
	ServerName->SetText(FText::FromString(Name));
}
