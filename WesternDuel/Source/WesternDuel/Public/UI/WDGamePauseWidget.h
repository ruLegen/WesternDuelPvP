// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDGamePauseWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class WESTERNDUEL_API UWDGamePauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(meta=(BindWidget))
	UButton* MenuGameButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackGameButton;
private:
	
	UFUNCTION()
	void OnMenuButtonPressed();
	
	UFUNCTION()
	void OnBackButtonPressed();
};
