// Fill out your copyright notice in the Description page of Project Settings.


#include "WDGameModeBase.h"
#include "WDGameStateBase.h"
#include "WDGameInstance.h"

#include "Character/WDPlayerController.h"
#include "Character/WDGamePlayerState.h"
#include "Character/WD_BaseCharacter.h"
#include "UI/WDCrosshairActor.h"
#include "UI/WDGameHUD.h"
#include "Weapon/WDWeaponBase.h"
#include "Enviroment/Pickups/WDPickUpBase.h"

#include "Multiplayer/WDPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"



AWDGameModeBase::AWDGameModeBase()
{
	PlayerControllerClass = AWDPlayerController::StaticClass();
	GameStateClass = AWDGameStateBase::StaticClass();
	HUDClass = AWDGameHUD::StaticClass();
	PlayerStateClass = AWDGamePlayerState::StaticClass();
}
void AWDGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void AWDGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	HandleNewPLayer(NewPlayer);
}

void AWDGameModeBase::HandleSeamlessTravelPlayer(AController*& NewPlayer)
{
	Super::HandleSeamlessTravelPlayer(NewPlayer);
	auto NewPlayerControler = Cast<APlayerController>(NewPlayer);
	if (!NewPlayerControler) return;
	HandleNewPLayer(NewPlayerControler);
}
/*
*  Track new logged in PlayerController
*/
void AWDGameModeBase::HandleNewPLayer(APlayerController* NewPlayer)
{
	AWDPlayerController* NewPlayerController = Cast<AWDPlayerController>(NewPlayer);
	if (!NewPlayerController) return;

	SetPause(NewPlayerController);
	Players.Add(NewPlayerController);
	if (Players.Num() == MaxPlayers)
	{
		ClearPause();
		auto CurrentGameState = GetWorld()->GetGameState<AWDGameStateBase>();
		if (!CurrentGameState) return;

		CurrentGameState->StartRound();
	}
}

void AWDGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AWDPlayerController* NewPlayerController = Cast<AWDPlayerController>(Exiting);
	if (!NewPlayerController) return;

	Players.Remove(NewPlayerController);
	Exiting->Destroy();
}

void AWDGameModeBase::FinishGame()
{
	for (auto Player : Players)
	{
		Player->Disconnect();			// Tell all clients to disconnect
	}
	// if we are lsiten server. Disconnect local player controller
	auto GameInstance = GetGameInstance<UWDGameInstance>();
	if (GameInstance)
	{
		GameInstance->DestroySession();
	}
	GetWorld()->ServerTravel(UWDGameInstance::MainMenuLevel, true, true);
}



/*
* Befor restarting round, we should free all PlayerStart
* NextRound - indicate which PlayerStart's we should use for spawning
*/
void AWDGameModeBase::RestartRound(int NextRound)
{
	ResetPlayerStarts();
	CurrentRound = NextRound;
	UWorld* World = GetWorld();

	for (auto Player : Players)
	{
		if (Player && Player->GetPawn())
		{
			Player->GetPawn()->Reset();
		}
	}
	ClearWorld();
	ResetPickups();
	for (auto Player : Players)
	{
		Player->StartSpot = nullptr;
		RestartPlayer(Player);
	}

}

void AWDGameModeBase::ResetPlayerStarts()
{
	TArray<AActor*> AllStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWDPlayerStart::StaticClass(), AllStarts);

	for (auto Start : AllStarts)
	{
		auto CastedStart = Cast<AWDPlayerStart>(Start);
		if (CastedStart)
		{
			CastedStart->SetOccupition(false);
		}
	}
}


void AWDGameModeBase::ResetPickups()
{
	TArray<AActor*> AllPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWDPickUpBase::StaticClass(), AllPickups);

	for (auto Pickup : AllPickups)
	{
		if (Pickup)
		{
			Pickup->Reset();
		}
	}
}

AActor* AWDGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
		//ChoosePlayerStart_Implementation(Player);
}

/*
* Almost the same as Parent Class,
* Just instead of looking for APlayerStart we look for AWDPlayerStart
*/
AActor* AWDGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!Cast<AWDPlayerController>(Player))
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
	UWorld* World = GetWorld();
	AActor* BestStart = nullptr; //ChoosePlayerStart(Player);
	TArray<AWDPlayerStart*> AllStarts = GetPlayerStartByRound(CurrentRound);
	for (auto Start : AllStarts)
	{																					    
		if (!Start->IsOccupied())
		{
			Start->bIsOccupied = true;
			BestStart = Start;
			break;         
		}
	}

	if (BestStart == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is nothing found, trying call super's function"));
		BestStart = Super::ChoosePlayerStart_Implementation(Player);
	}
	return BestStart;
}


TArray<AWDPlayerStart*> AWDGameModeBase::GetPlayerStartByRound(int32 Round)
{
	TArray<AWDPlayerStart*> Result;
	UWorld* World = GetWorld();
	for (TActorIterator<AWDPlayerStart> It(World); It; ++It)
	{
		AWDPlayerStart* Start = *It;
		if (Start && Start->GetRoundNumber() == Round)
		{
			Result.Add(Start);
		}
	}
	return Result;
}
/*
* Remove all bodies and weapons
*/
void AWDGameModeBase::ClearWorld()
{
	RemoveAllActorsByClass<AWD_BaseCharacter>();
	RemoveAllActorsByClass<AWDWeaponBase>();
}

UClass* AWDGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// TODO Implement different Pawn for AIControllers
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}


void AWDGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

}