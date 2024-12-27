// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WDWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "UI/WDCrosshairActor.h"
#include "Weapon/WDWeaponBase.h"
#include "WDGameStateBase.h"


// Sets default values for this component's properties
UWDWeaponComponent::UWDWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWDWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

void UWDWeaponComponent::BeginDestroy()
{
	DestroyWeapon();
	Super::BeginDestroy();
}
void UWDWeaponComponent::DestroyWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}

void UWDWeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	CurrentWeapon = GetWorld()->SpawnActor<AWDWeaponBase>(WeaponClass);
	if (!CurrentWeapon) return;
	CurrentWeapon->OnEmpty.AddUObject(this, &UWDWeaponComponent::OnEmptyGun);
	CurrentWeapon->SetOwner(GetOwner());
	HolstersWeapon();					// Put gun in holster

}

void UWDWeaponComponent::OnEmptyGun()
{
	if (!GetWorld()) return;

	if (GetWorld() && CurrentWeapon)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, [&]() {
		
			if (CurrentWeapon->IsEmpty())
			{

				auto CurrentGameState = GetWorld()->GetGameState<AWDGameStateBase>();
				if (!CurrentGameState) return;

				CurrentGameState->NotifyEmpty();
			}
		}, 0.5, false);
	}
}
void UWDWeaponComponent::AttachWeaponTo(AWDWeaponBase* Weapon, USceneComponent* Mesh, FName SocketName)
{
	if (!Weapon) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
	Weapon->SetOwner(GetOwner());
}
void UWDWeaponComponent::TakeGunInHand() {
	if (!GetWorld() || !CurrentWeapon) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	AttachWeaponTo(CurrentWeapon, Character->GetMesh(), HandSocketName);
}

AWDWeaponBase* UWDWeaponComponent::GetWeapon() {
	return CurrentWeapon;
}

void UWDWeaponComponent::HolstersWeapon() {
	if (!GetWorld() || !CurrentWeapon) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	AttachWeaponTo(CurrentWeapon, Character->GetMesh(), HolsterSocketName);
}
void UWDWeaponComponent::Fire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->Fire();
}
void UWDWeaponComponent::Detach() {
	 CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	 CurrentWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	 CurrentWeapon->GetMesh()->SetAllBodiesSimulatePhysics(true);
	 CurrentWeapon->GetMesh()->SetEnablePhysicsBlending(true);
}
void UWDWeaponComponent::SetCrosshairVisibility(bool Visible) {
	CurrentWeapon->SetCrosshairDrawing(Visible);
}

void UWDWeaponComponent::AddAmmo_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ResetAmmo();
	}
}