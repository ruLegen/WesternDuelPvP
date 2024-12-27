// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/Pickups/WDHealthPickUp.h"
#include "GameFramework/Character.h"
#include "Components/WDHealthComponent.h"
#include "Net/UnrealNetwork.h"



void AWDHealthPickUp::OnPickedUp_Implementation(ACharacter* Character)
{
	auto HealthComponent = Cast<UWDHealthComponent>(Character->GetComponentByClass(UWDHealthComponent::StaticClass()));
	if (!HealthComponent) return;
	
	Heal(HealthComponent);

};


void AWDHealthPickUp::Heal_Implementation(UWDHealthComponent* HealthComponent)
{
	HealthComponent->Heal(HealAmount, true);
}


void AWDHealthPickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDHealthPickUp, HealAmount);

}
