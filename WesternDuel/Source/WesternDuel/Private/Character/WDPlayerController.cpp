// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WDPlayerController.h"
#include "Character/WD_BaseCharacter.h"
#include "UI/WDGameHUD.h"
#include "WDGameStateBase.h"
#include "WDGameInstance.h"
#include "Kismet/GameplayStatics.h"



AWDPlayerController::AWDPlayerController() 
{
	bReplicates = true;
}

void AWDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AWDPlayerController::OnPausePressed);
}

void AWDPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	if (!GetWorld()) return;


	auto CurrentGameState = GetWorld()->GetGameState<AWDGameStateBase>();
	if (CurrentGameState)
	{
		CurrentGameState->OnRoundStart.AddUObject(this, &AWDPlayerController::OnStartRound);
		CurrentGameState->OnRoundChanged.AddUObject(this, &AWDPlayerController::OnRestartRound);
		CurrentGameState->OnRoundEnd.AddUObject(this, &AWDPlayerController::OnEndRound);
		CurrentGameState->OnGameOver.AddUObject(this, &AWDPlayerController::OnGameOver);
	}
};

void AWDPlayerController::OnRestartRound()
{
	auto GameHUD = GetHUD<AWDGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Round Changed"));
	GameHUD->SetState(EHUDState::NONE);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
void AWDPlayerController::OnStartRound()
{
	auto ControllingPawn = GetPawn<AWD_BaseCharacter>();
	if (!ControllingPawn) return;

	ControllingPawn->Server_SetCanAim(true);
}
void AWDPlayerController::OnEndRound()
{
	auto GameHUD = GetHUD<AWDGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Round End"));
	GameHUD->SetState(EHUDState::ROUND_END);
}

void AWDPlayerController::OnGameOver()
{
	auto GameHUD = GetHUD<AWDGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	GameHUD->SetState(EHUDState::GAMEOVER);

	DisableInput(this);
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AWDPlayerController::OnPausePressed()
{
	auto GameHUD = GetHUD<AWDGameHUD>();
	if (!GameHUD) return;

	EHUDState CurrentState = GameHUD->GetHUDState();
	if (CurrentState == EHUDState::PAUSE)
	{
		if (PrevHUDState == EHUDState::NONE)
		{
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
		GameHUD->SetState(PrevHUDState);
		PrevHUDState = CurrentState;
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
		PrevHUDState = CurrentState;
		GameHUD->SetState(EHUDState::PAUSE);
	}


}

void AWDPlayerController::Disconnect_Implementation()
{
	if(!HasAuthority())
	GEngine->HandleDisconnect(GetWorld(), GetWorld()->GetNetDriver());
}