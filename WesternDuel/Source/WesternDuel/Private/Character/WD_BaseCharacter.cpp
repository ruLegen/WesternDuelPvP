// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WD_BaseCharacter.h"
#include "WDGameStateBase.h"
#include "Character/WDPlayerController.h"
#include "Character/WDGamePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "UI/WDCrosshairActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WDWeaponComponent.h"
#include "Components/WDHealthComponent.h"
#include "Weapon/WDWeaponBase.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AWD_BaseCharacter::AWD_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bReplicates = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm Component"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 160;
	SpringArmComponent->SocketOffset = FVector(-20, 0, 60);
	SpringArmComponent->TargetOffset = FVector(0,0,10);

	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	SphereOverHead = CreateDefaultSubobject<USphereComponent>(TEXT("OverHeadArea"));
	SphereOverHead->InitSphereRadius(30);
	SphereOverHead->SetCollisionProfileName(TEXT("OverHead"));
	SphereOverHead->SetupAttachment(GetMesh());

	WeaponComponent = CreateDefaultSubobject<UWDWeaponComponent>(TEXT("Weapon"));
	HealthComponent = CreateDefaultSubobject<UWDHealthComponent>(TEXT("Health"));

	GetCharacterMovement()->bUseControllerDesiredRotation = 0;
	GetCharacterMovement()->bOrientRotationToMovement = 0;
}

// Called when the game starts or when spawned
void AWD_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	if(HasAuthority())
		HealthComponent->OnDie.AddUObject(this, &AWD_BaseCharacter::OnDie);
	WeaponComponent->SetIsReplicated(true);
	GetCapsuleComponent()->SetIsReplicated(true);
	GetMesh()->SetIsReplicated(true);
	GetMesh()->SetCollisionProfileName(TEXT("PhysicsBody"));
	WeaponComponent->SetCrosshairVisibility(false);
	
}
void AWD_BaseCharacter::UnPossessed()
{
	DeleteWeapon();
	Super::UnPossessed();
}
// Called every frame
void AWD_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled())
		UpdateAimDirection(GetBaseAimRotation());
}

// Called to bind functionality to input
void AWD_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, AWD_BaseCharacter::Dodge);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AWD_BaseCharacter::Server_TryFire);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AWD_BaseCharacter::Server_StartAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AWD_BaseCharacter::Server_StopAim);
	PlayerInputComponent->BindAction("TakeGun", EInputEvent::IE_Pressed, this, &AWD_BaseCharacter::Server_RequestTakeGun);
	PlayerInputComponent->BindAction("TakeGun", EInputEvent::IE_Released, this, &AWD_BaseCharacter::Server_RequestPutBackGun);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWD_BaseCharacter::TryMoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AWD_BaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookAround", this, &AWD_BaseCharacter::LookAround);

	
}

float AWD_BaseCharacter::GetMovementDirection()
{
	float Direction = GetVelocity().Size()*InputDirection;
	return Direction;
}
FRotator AWD_BaseCharacter::GetAimDirection() 
{
	FRotator AimDiff = GetAimOffsets();
	AimDiff.Yaw = UKismetMathLibrary::NormalizeAxis(AimDiff.Yaw);
	return AimDiff;
}
void AWD_BaseCharacter::UpdateAimDirection_Implementation(FRotator ClientAimRotation)
{
	AimRotator = ClientAimRotation;
}

//
FRotator AWD_BaseCharacter::GetAimOffsets() 
{
	const FVector AimDirWS = AimRotator.Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}
void AWD_BaseCharacter::TryMoveRight(float Value) 
{
	if (!bGunIsTaken) {
		AddMovementInput(GetActorRightVector(), Value);
		MoveRight(Value);
	}
}
void AWD_BaseCharacter::MoveRight_Implementation(float Value)
{
	 InputDirection = Value;
}
void AWD_BaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AWD_BaseCharacter::LookAround(float Value)
{
	AddControllerYawInput(Value);
}

void AWD_BaseCharacter::Server_TryFire_Implementation()
{
	if (!WeaponComponent) return;
	if(bGunIsTaken)
		WeaponComponent->Fire();
}


void AWD_BaseCharacter::Server_RequestTakeGun_Implementation()
{
	bGunIsTaken = true;
	TakeGun();
}
void AWD_BaseCharacter::TakeGun_Implementation()
{

	WeaponComponent->TakeGunInHand();
	if (bGunIsTaken && bCanAim)
		bIsAiming = true;
}

void AWD_BaseCharacter::Server_RequestPutBackGun_Implementation()
{
	bGunIsTaken = false;
	bIsAiming = false;
	PutBackGun();
}
void AWD_BaseCharacter::PutBackGun_Implementation()
{
	WeaponComponent->HolstersWeapon();
}

void AWD_BaseCharacter::Server_StartAim_Implementation()
{
	if(bGunIsTaken && bCanAim)
		bIsAiming = true;
}
void AWD_BaseCharacter::Server_StopAim_Implementation()
{
	bIsAiming = false;
}

void AWD_BaseCharacter::Server_SetCanAim_Implementation(bool bCan)
{
	bCanAim = bCan;
	OnRep_CanAim();
}

// Auto Aim if Player already have taken gun
void AWD_BaseCharacter::OnRep_CanAim()
{
	if(bGunIsTaken && bIsAutoAim)
		Server_StartAim();
}
// On Server
void AWD_BaseCharacter::OnDie(AController* KilledBy)
{
		auto CurrentPlayerGameState = GetPlayerState<AWDGamePlayerState>();
		if (CurrentPlayerGameState)
		{
			CurrentPlayerGameState->AddDeath(1);
		}
		if (KilledBy)
		{
			auto KillerPlayerGameState = KilledBy->GetPlayerState<AWDGamePlayerState>();
			if (KillerPlayerGameState && GetController() != KilledBy)
			{
				KillerPlayerGameState->AddKill(1);
			}
		}
	KillCharacter();
	GetWorld()->GetGameState<AWDGameStateBase>()->NextRound();
}

void AWD_BaseCharacter::KillCharacter_Implementation() {
	WeaponComponent->Detach();
	WeaponComponent->SetCrosshairVisibility(false);
	DeleteWeapon();

	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetEnablePhysicsBlending(true);
	
	GetMesh()->AddForceAtLocation(-GetActorForwardVector() * 3000000, GetActorLocation(), "spine_01");
	
	
	const auto PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	auto Spectator = GetWorld()->SpawnActor<ASpectatorPawn>();
	if (Spectator && HasAuthority())
	{
		Spectator->TeleportTo(GetActorLocation(), GetActorRotation(), false, true);
		PlayerController->Possess(Spectator);
	}
}

void AWD_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWD_BaseCharacter, bGunIsTaken);
	DOREPLIFETIME(AWD_BaseCharacter, AimRotator);
	DOREPLIFETIME(AWD_BaseCharacter, bIsAiming);
	DOREPLIFETIME(AWD_BaseCharacter, HealthComponent);
	DOREPLIFETIME(AWD_BaseCharacter, InputDirection);
	DOREPLIFETIME(AWD_BaseCharacter, WeaponComponent);
	DOREPLIFETIME(AWD_BaseCharacter, bCanAim);
	DOREPLIFETIME(AWD_BaseCharacter, bIsAutoAim);
};


void AWD_BaseCharacter::DeleteWeapon_Implementation()
{
	WeaponComponent->GetWeapon()->Destroy();
}

