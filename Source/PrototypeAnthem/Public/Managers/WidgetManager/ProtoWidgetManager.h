// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoScreenWidgetData.h"
#include "Game/ProtoGameState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "ProtoWidgetManager.generated.h"

class UProtoLobby_Widget;
class UProtoMission_Widget;
class UProtoScreenWidgetData;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Config=Game, DefaultConfig)
class PROTOTYPEANTHEM_API UProtoWidgetManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Set up manager with owning controller */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="UI")
	void SetCachedOwner(APlayerController* PC);
	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowScreenWidget();

	/** Removes current main widget */
	UFUNCTION(BlueprintCallable, Category="UI")
	void RemoveCurrentScreenWidget();
	UFUNCTION()
	void SpawnWidgetsForScreenWidgetData(EGameContext Screen);

protected:

	UPROPERTY(EditDefaultsOnly, Category="UI")
	APlayerController* CachedOwner;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TArray<UUserWidget*> ActiveScreenWidgets;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TArray<FAssetData> ScreenWidgetsData;

	UPROPERTY()
	EGameContext CurrentScreen = EGameContext::None;
};
