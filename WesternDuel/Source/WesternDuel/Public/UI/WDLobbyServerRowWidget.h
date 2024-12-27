// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDLobbyServerRowWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_OneParam(FRowClickedSignature,int32)
UCLASS()
class WESTERNDUEL_API UWDLobbyServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta=(BindWidget))
	UButton* ServerButton;
	
	UPROPERTY()
	int32 Index;

public:
	FRowClickedSignature OnClicked;

	UFUNCTION()
	void OnSelect();

	void SetServerName(FString Name);
	void SetIndex(int32 Id) { Index = Id; }

};
