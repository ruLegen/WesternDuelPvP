// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WDWeaponComponent.generated.h"

class AWDWeaponBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WESTERNDUEL_API UWDWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWDWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName HolsterSocketName = "Weapon_Holster";
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName HandSocketName= "Weapon_Socket";


	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AWDWeaponBase> WeaponClass;

	UPROPERTY()
	AWDWeaponBase* CurrentWeapon;
	
	FTimerHandle Timer;

	void AttachWeaponTo(AWDWeaponBase* Weapon, USceneComponent* Mesh, FName SocketName);

public:	
	void SpawnWeapon();
	void TakeGunInHand();
	void HolstersWeapon();
	void Fire();
	AWDWeaponBase* GetWeapon();
	void Detach();
	void SetCrosshairVisibility(bool Visible);
	void DestroyWeapon();

	UFUNCTION(Server, Reliable)
	void AddAmmo();			// Reset Ammo to Max available
private:
	UFUNCTION()
	void OnEmptyGun();
};
