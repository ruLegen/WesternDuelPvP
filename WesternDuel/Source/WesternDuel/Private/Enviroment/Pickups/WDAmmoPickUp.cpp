// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/Pickups/WDAmmoPickUp.h"

#include "GameFramework/Character.h"
#include "Components/WDWeaponComponent.h"
#include "Net/UnrealNetwork.h"



void AWDAmmoPickUp::OnPickedUp_Implementation(ACharacter* Character)
{
	auto WeaponComponent = Cast<UWDWeaponComponent>(Character->GetComponentByClass(UWDWeaponComponent::StaticClass()));
	if (!WeaponComponent) return;
	
	AddAmmo(WeaponComponent);
};

void AWDAmmoPickUp::AddAmmo_Implementation(UWDWeaponComponent* WeaponComponent)
{
	WeaponComponent->AddAmmo();
}

