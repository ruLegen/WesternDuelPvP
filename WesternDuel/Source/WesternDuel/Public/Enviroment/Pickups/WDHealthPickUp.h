// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enviroment/Pickups/WDPickUpBase.h"
#include "WDHealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API AWDHealthPickUp : public AWDPickUpBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,Replicated)
	float HealAmount;
	
	UFUNCTION(Server, Reliable)
	void Heal(UWDHealthComponent* HealthComponent);
	virtual void OnPickedUp_Implementation(ACharacter* Character) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
