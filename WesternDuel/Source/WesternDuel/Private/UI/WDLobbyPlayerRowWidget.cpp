// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDLobbyPlayerRowWidget.h"
#include "Components/TextBlock.h"

void UWDLobbyPlayerRowWidget::SetName(FText Name)
{
	if (PlayerNameText)
		PlayerNameText->SetText(Name);
}
