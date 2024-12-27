// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WDMainMenuHUD.generated.h"

class UUserWidget;
class UWDMainMenuWidget;
class UWDLobbySearchWidget;
UCLASS()
class WESTERNDUEL_API AWDMainMenuHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbySearchWidgetClass;


	UWDMainMenuWidget* MenuWidget;
	UWDLobbySearchWidget* LobbyWidget;
private:
	void OnJoinRequest();
};
