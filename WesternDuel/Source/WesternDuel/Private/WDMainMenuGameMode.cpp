// Fill out your copyright notice in the Description page of Project Settings.


#include "WDMainMenuGameMode.h"
#include "UI/WDMainMenuHUD.h"
#include "Character/WDMainMenuPlayerController.h"

AWDMainMenuGameMode::AWDMainMenuGameMode() {
	HUDClass = AWDMainMenuHUD::StaticClass();
	PlayerControllerClass = AWDMainMenuPlayerController::StaticClass();
}
