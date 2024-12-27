// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/Pickups/WDPickUpBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/WDProjectTileActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"


// Sets default values
AWDPickUpBase::AWDPickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Root");
	SphereComponent->InitSphereRadius(30);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);	// Ignore Capsule

	RootComponent = SphereComponent;

	MeshComponent= CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;

}

void AWDPickUpBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &AWDPickUpBase::OnHit);
	}
}

void AWDPickUpBase::Reset_Implementation()
{
	bIsHidden = false;
	OnStateChanged();
}

void AWDPickUpBase::OnStateChanged()
{
	SphereComponent->SetCollisionEnabled(bIsHidden? ECollisionEnabled::NoCollision: ECollisionEnabled::QueryOnly);
	MeshComponent->SetVisibility(!bIsHidden);
}

void AWDPickUpBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsHidden)
	{
		AWDProjectTileActor* Proejectile = Cast<AWDProjectTileActor>(OtherActor);
		if (!Proejectile) return;

		auto OwningCharacter = Cast<ACharacter>(Proejectile->GetInsigatorController()->GetPawn());
		if (OwningCharacter)
		{
			OnPickedUp_Implementation(OwningCharacter);
			PlayPickUpSound(Hit.ImpactPoint);
			bIsHidden = true;
			OnStateChanged();
		}
	}
}

void AWDPickUpBase::PlayPickUpSound_Implementation(FVector Location)
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, Location);
	}
}
void AWDPickUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWDPickUpBase, bIsHidden);
}
