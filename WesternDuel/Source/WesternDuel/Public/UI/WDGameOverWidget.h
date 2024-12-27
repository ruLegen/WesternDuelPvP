// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDGameOverWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;

UCLASS()
class WESTERNDUEL_API UWDGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* KillsTextBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsTextBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameOverTextBox;

	
	

public:

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	void SetKills(int32 Kills);
	void SetDeaths(int32 Deaths);
};
