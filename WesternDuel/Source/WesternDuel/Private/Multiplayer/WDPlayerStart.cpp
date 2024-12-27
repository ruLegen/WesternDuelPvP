// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/WDPlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AWDPlayerStart::AWDPlayerStart(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bReplicates= true;
	GetCapsuleComponent()->SetMobility(EComponentMobility::Movable);

}

void AWDPlayerStart::BeginPlay()
{
	Super::BeginPlay();
}
void AWDPlayerStart::SetOccupition_Implementation(bool IsOccupied)
{
	bIsOccupied = IsOccupied;
}

void AWDPlayerStart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDPlayerStart, bIsOccupied);
}