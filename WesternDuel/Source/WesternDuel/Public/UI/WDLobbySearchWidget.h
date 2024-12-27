// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDLobbySearchWidget.generated.h"

/**
 * 
 */
class FOnlineSessionSearchResult;
class UScrollBox;
class UButton;
UCLASS()
class WESTERNDUEL_API UWDLobbySearchWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerRowClass;


	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerListScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;


protected:
	UFUNCTION()
	void Refresh();
	void OnRefresh(TArray<FOnlineSessionSearchResult>& Servers);
	void UpdateServers(TArray<FOnlineSessionSearchResult>& Servers);

	void OnRowClicked(int32 Id);
};
