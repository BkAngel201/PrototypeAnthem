// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProtoLobby_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API UProtoLobby_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category="Lobby")
	void UpdatePlayerStatus();

	UFUNCTION(BlueprintImplementableEvent, Category="Lobby")
	void StartCountdown(float Time);

	UFUNCTION(BlueprintImplementableEvent, Category="Lobby")
	void ShowMissionTransition();
};
