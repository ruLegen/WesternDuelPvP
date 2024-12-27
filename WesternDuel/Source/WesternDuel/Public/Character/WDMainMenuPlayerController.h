// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WDMainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API AWDMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

};
