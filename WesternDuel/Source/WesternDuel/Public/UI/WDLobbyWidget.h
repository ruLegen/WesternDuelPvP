// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDLobbyWidget.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class UWDLobbyPlayerRowWidget;
class UHorizontalBox;
UCLASS()
class WESTERNDUEL_API UWDLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UScrollBox *PlayerScrollBox;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPlayerNum;


	UPROPERTY(EditDefaultsOnly,BLueprintReadWrite, Category="Row")
	TSubclassOf< UWDLobbyPlayerRowWidget> RowClass;


public:
	UFUNCTION()
	void StartGame();
	void UpdatePlayers(TArray<FString> Players);
	void SetButtonVisibility(bool Visible);
};
