// Copyright Epic Games, Inc. All Rights Reserved.


#include "WesternDuelGameModeBase.h"
#include "Character/WDPlayerController.h"

AWesternDuelGameModeBase::AWesternDuelGameModeBase() {
	PlayerControllerClass = AWDPlayerController::StaticClass();
}
