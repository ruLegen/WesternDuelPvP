

#include "Enviroment/WDEnviromentSound.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "WDGameStateBase.h"


AWDEnviromentSound::AWDEnviromentSound()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AWDEnviromentSound::BeginPlay()
{
	Super::BeginPlay();
	auto GameState = GetWorld()->GetGameState<AWDGameStateBase>();
	if (GameState)
	{
		GameState->OnRoundStart.AddUObject(this, &AWDEnviromentSound::OnRoundStart);
	}
}


void AWDEnviromentSound::OnRoundStart_Implementation()
{
	int32 RandIndex = FMath::Rand() % SoundArray.Num();
	auto Sound = SoundArray[RandIndex].Sound;
	auto Location = SoundArray[RandIndex].Location;

	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
	}
}
