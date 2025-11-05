// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProtoMission_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API UProtoMission_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Mission")
	void StartCountdown(float Time);
};
