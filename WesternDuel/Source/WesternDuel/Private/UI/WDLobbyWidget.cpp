// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDLobbyWidget.h"
#include "UI/WDLobbyPlayerRowWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "WDGameInstance.h"
#include "GameFramework/PlayerState.h"

void UWDLobbyWidget::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	if (StartGameButton) {
		if (GetOwningPlayer()->HasAuthority()) {
			StartGameButton->OnClicked.AddDynamic(this, &UWDLobbyWidget::StartGame);
		}
		else {
			StartGameButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UWDLobbyWidget::StartGame()
{
	GetWorld()->ServerTravel("/Game/Levels/Game/MainLevel?game=/Game/World/BP_MyWDGameModeBase.BP_MyWDGameModeBase_C?Listen");
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
	{
		ViewportClient->RemoveAllViewportWidgets();
	}
}

void UWDLobbyWidget::UpdatePlayers(TArray<FString> Players)
{
	
	if (TextPlayerNum)
	{
		FString NumText = FString::FromInt(Players.Num()) + " / 2";
		TextPlayerNum->SetText(FText::FromString(NumText));		// TODO Replace 2 with correct Max player value; But It is issue on client side
	}
	PlayerScrollBox->ClearChildren();
	for (auto PlayerName : Players) {
		auto PlayerRow = CreateWidget<UWDLobbyPlayerRowWidget>(GetWorld(),RowClass);
		if (!PlayerRow) continue;
		FText Name = FText::FromString(PlayerName.ToUpper());
		PlayerRow->SetName(Name);

		PlayerScrollBox->AddChild(PlayerRow);
	}
}

void UWDLobbyWidget::SetButtonVisibility(bool Visible)
{
	if (!StartGameButton) return;
	StartGameButton->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
