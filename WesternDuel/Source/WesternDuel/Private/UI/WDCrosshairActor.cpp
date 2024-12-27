// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDCrosshairActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

AWDCrosshairActor::AWDCrosshairActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	bReplicates = true;

}

void AWDCrosshairActor::BeginPlay()
{
	Super::BeginPlay();
	WidgetComponent->SetWidgetClass(CrosshairWidgetClass);
}
