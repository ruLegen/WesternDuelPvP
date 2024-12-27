// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WD_BaseCharacter.generated.h"

class UCameraComponent;
class USphereComponent;
class UWDWeaponComponent;
class UWDHealthComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
UCLASS()
class WESTERNDUEL_API AWD_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWD_BaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void UnPossessed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	USphereComponent* SphereOverHead;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Weapon")
	UWDWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Weapon")
	UWDHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
	bool bGunIsTaken = false;
	
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_CanAim, Category = "Player")
	bool bCanAim = false;
	
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
	bool bIsAiming = false;
	
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Player")
	bool bIsAutoAim= false;

	UPROPERTY(Replicated)
	float InputDirection = 0;
	
	UPROPERTY(Replicated)
	FRotator AimRotator;


public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	float GetMovementDirection();
	
	UFUNCTION(BlueprintCallable)
	bool HasGunTaken() { return bGunIsTaken; }

	UFUNCTION(BlueprintCallable)
	bool IsAim() { return bIsAiming; };
	
	UFUNCTION(BlueprintCallable)
	FRotator GetAimDirection();

	UFUNCTION(Server, unreliable)
	void UpdateAimDirection(FRotator ClientAimRotation);

	UFUNCTION(Server, reliable)
	void Server_SetCanAim(bool bCan);


	
	UFUNCTION(BlueprintCallable)
	UWDHealthComponent* GetHealthComponent() { return HealthComponent; };

	FRotator GetAimOffsets();


private:
	UFUNCTION(Server, unreliable)
	void MoveRight(float Value);

	void TryMoveRight(float Value);
	void LookUp(float Value);

	void LookAround(float Value);
	
	UFUNCTION(Server, unreliable)
	void Server_TryFire();
	
	UFUNCTION(Server, reliable)
	void Server_RequestTakeGun();
	

	UFUNCTION(NetMulticast, reliable)
	void TakeGun();

	UFUNCTION(Server, reliable)
	void Server_RequestPutBackGun();

	UFUNCTION(NetMulticast, reliable)
	void PutBackGun();
	
	UFUNCTION(Server, reliable)
	void Server_StartAim();
	
	UFUNCTION(Server, reliable)
	void Server_StopAim();
	
	UFUNCTION()
	void OnDie(AController* KilledBy);

	UFUNCTION(NetMulticast, reliable)
	void KillCharacter();

	UFUNCTION(NetMulticast, reliable)
	void DeleteWeapon();
	
	UFUNCTION()
	void OnRep_CanAim();


};
