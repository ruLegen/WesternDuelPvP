// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WDGameHUD.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EHUDState :uint8
{
	NONE,
	ROUND_END,
	GAMEOVER,
	PAUSE,
	MAX
};

class UWDGameOverWidget;
class UWDRoundResultWidget;
UCLASS()
class WESTERNDUEL_API AWDGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RoundResultWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

public:
	UFUNCTION()
	void UpdatePlayerInfo();
	
	UFUNCTION()
	void SetState(EHUDState State);

	EHUDState GetHUDState() { return HUDState; };
	void InitAll();
	void HideAll();

private:
	EHUDState HUDState = EHUDState::NONE;
	TMap<EHUDState, UUserWidget*> HUDWidgetMap;

	void SetStateVisibility(EHUDState State);
	
	UFUNCTION()
	void OnMainMenuClicked();
	
	UFUNCTION()
	void OnBackClicked();
	
	UFUNCTION()
	void OnRestartMatchClicked();
};
