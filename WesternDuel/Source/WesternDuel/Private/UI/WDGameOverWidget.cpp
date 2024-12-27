// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WDGameOverWidget.h"
#include "Components/TextBlock.h"


void UWDGameOverWidget::SetKills(int32 Kills)
{
	KillsTextBox->SetText(FText::FromString("Kills " + FString::FromInt(Kills)));
}

void UWDGameOverWidget::SetDeaths(int32 Deaths)
{
	DeathsTextBox->SetText(FText::FromString("Deaths " + FString::FromInt(Deaths)));
}
