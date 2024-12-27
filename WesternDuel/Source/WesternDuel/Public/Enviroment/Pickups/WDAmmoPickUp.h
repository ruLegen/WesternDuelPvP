// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enviroment/Pickups/WDPickUpBase.h"
#include "WDAmmoPickUp.generated.h"

/**
 *
 */
class UWDWeaponComponent;
UCLASS()
class WESTERNDUEL_API AWDAmmoPickUp : public AWDPickUpBase
{
	GENERATED_BODY()
protected:


	UFUNCTION(Server, Reliable)
	void AddAmmo(UWDWeaponComponent* WeaponComponent);

	virtual void OnPickedUp_Implementation(ACharacter* Character) override;
};
