// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI_WDBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNDUEL_API UAI_WDBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float Direction;
};
