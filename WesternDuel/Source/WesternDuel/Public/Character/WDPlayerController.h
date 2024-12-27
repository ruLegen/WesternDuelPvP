// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/WDGameHUD.h"

#include "WDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API AWDPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	void OnStartRound();
	void OnEndRound();
	void OnRestartRound();
	void OnGameOver();
public:
	AWDPlayerController();
	virtual void SetupInputComponent() override;
	UFUNCTION(Client,reliable)
	void Disconnect();
private:
	EHUDState PrevHUDState = EHUDState::NONE;

	void OnPausePressed();
};
