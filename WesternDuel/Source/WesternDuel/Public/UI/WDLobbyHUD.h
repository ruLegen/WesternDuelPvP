// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WDLobbyHUD.generated.h"
class UWDLobbyWidget;
UCLASS()
class WESTERNDUEL_API AWDLobbyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	UWDLobbyWidget* LobbyWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UFUNCTION()
		void OnPlayersUpdate(TArray<FString> Players);
};
