// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WDWeaponBase.h"
#include "Weapon/WDProjectTileActor.h"
#include "Character/WD_BaseCharacter.h"
#include "UI/WDCrosshairActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraShake.h"
#include "Particles/ParticleSystemComponent.h"



AWDWeaponBase::AWDWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Root"));
	RootComponent = SkeletalMeshComponent;
	MuzzleSocketName = "Muzzle";
}

void AWDWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentWeaponData.CrosshairClass.Get())
	{
		Crosshair = GetWorld()->SpawnActor<AWDCrosshairActor>(CurrentWeaponData.CrosshairClass);
	}
}
void AWDWeaponBase::BeginDestroy()
{
	if(Crosshair)
		Crosshair->Destroy();
	Super::BeginDestroy();
}

void AWDWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ShouldDrawCrosshair&& Crosshair) {
		FHitResult Hit;
		FVector Start = GetMuzzleLocation();
		FVector End = GetMuzzleLocation() + GetActorForwardVector() * 3381.236643389053;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f, 0, 1);
		//// TODO Fix crosshair
		//GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Pawn);

		//if (Hit.bBlockingHit) {
		//	DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Cyan, false, 0.1f, 0, 1);
		//	End = Hit.ImpactPoint;
		//}
		Crosshair->SetActorLocation(End);
	}
}
bool AWDWeaponBase::IsEmpty()
{
	return CurrentWeaponData.CurrentBullets == 0;
}
void AWDWeaponBase::Fire_Implementation()
{
	if (GetWorld()) {
		if (!IsEmpty())
		{
			//Fire
			SpawnMuzzleFlash();
			PlaySound(FireSoundQue);
			PlayCameraShake();
			DecreaseAmmoBy(1);
			//Spawn ProjectTile

			FTransform ProjectileTransform = FTransform(FRotator::ZeroRotator, GetMuzzleLocation());

			auto ShotCharacter = Cast<ACharacter>(GetOwner());		// here is may be BUG if weaponComponent was not attached to Character
			if (ShotCharacter)
			{
				auto ProjectTile = GetWorld()->SpawnActorDeferred<AWDProjectTileActor>(CurrentWeaponData.ProjectileClass, ProjectileTransform, GetOwner());
				ProjectTile->SetShotDirection(GetActorForwardVector());
				ProjectTile->SetShotController(ShotCharacter->GetController());
				ProjectTile->FinishSpawning(ProjectileTransform);
			}
		}
		else
		{
			PlaySound(EmptySoundQue);
		}
		
	}
}


void AWDWeaponBase::PlaySound_Implementation(USoundCue* Sound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName));
	}
}
void AWDWeaponBase::DecreaseAmmoBy(int32 Num) {
	CurrentWeaponData.CurrentBullets = FMath::Clamp<int32>(CurrentWeaponData.CurrentBullets-Num,0, CurrentWeaponData.MaxBullets);
	if (IsEmpty())
	{
		OnEmpty.Broadcast();
	}
}
FVector AWDWeaponBase::GetMuzzleLocation() {
	return SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
}

USkeletalMeshComponent* AWDWeaponBase::GetMesh()
{
	return SkeletalMeshComponent;
}

void  AWDWeaponBase::SetCrosshairDrawing(bool IsDrawing) {
	ShouldDrawCrosshair = IsDrawing;
	if (Crosshair)
	{
		Crosshair->SetActorHiddenInGame(!IsDrawing);
	}
}


void AWDWeaponBase::PlayCameraShake_Implementation()
{
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;

	if (CameraShake)
		Controller->PlayerCameraManager->StartCameraShake(CameraShake);

}
void AWDWeaponBase::ResetAmmo_Implementation()
{
	CurrentWeaponData.CurrentBullets = CurrentWeaponData.MaxBullets;
}


void AWDWeaponBase::SpawnMuzzleFlash_Implementation()
{
	//if (SkeletalMeshComponent && MuzzleFlash)
//	{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkeletalMeshComponent, MuzzleSocketName, GetMuzzleLocation(), SkeletalMeshComponent->GetSocketRotation(MuzzleSocketName), EAttachLocation::KeepWorldPosition);
	//	}
}

void AWDWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDWeaponBase, SkeletalMeshComponent);
	DOREPLIFETIME(AWDWeaponBase, CurrentWeaponData);
	DOREPLIFETIME(AWDWeaponBase, ShouldDrawCrosshair);
}

