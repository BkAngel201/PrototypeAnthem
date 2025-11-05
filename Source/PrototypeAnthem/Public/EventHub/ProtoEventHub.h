// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/ProtoTypes.h"
#include "Debug/ProtoDebug.h"
#include "Managers/MissionManager/Missions/ProtoMissionDefinition.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProtoEventHub.generated.h"

enum class EMissionState : uint8;
enum class EObjectiveState : uint8;
enum class EAttribute : uint8;
class UProtoMissionManager;
class UProtoObjectiveBase;
/**
 * 
 */

// Attribute System
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, AffectedActor, EAttribute, Attribute, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, AActor*, DeadActor);

// Mission Flow
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEHubMissionUpdated, FMissionRuntimeData, MissionData);

//Countdown
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScreenCountdownStarted, float, CountdownEndTime);

UCLASS(Blueprintable)
class PROTOTYPEANTHEM_API UProtoEventHub : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Gameplay|Events")
	FOnEHubMissionUpdated OnMissionUpdated;

	UPROPERTY(BlueprintAssignable, Category="Gameplay|Events")
	FOnScreenCountdownStarted OnScreenCountdownStarted;

	UPROPERTY(BlueprintAssignable, Category="Gameplay|Events")
	FOnAttributeChanged OnAttributeChanged;
	UPROPERTY(BlueprintAssignable, Category="Gameplay|Events")
	FOnCharacterDied OnCharacterDied;

	UFUNCTION()
	void BroadcastMissionUpdated(FMissionRuntimeData MissionData)
	{
		DEBUG_LOG("EventBus: BroadcastMissionUpdated() - Mission Data Changed");
		OnMissionUpdated.Broadcast(MissionData);
	}
	UFUNCTION()
	void BroadcastScreenCountdownStarted(float CountdownEndTime)
	{
		DEBUG_LOG("EventBus: BroadcastScreenCountdownStarted() - CountdownEndTime set to %f", CountdownEndTime);
		OnScreenCountdownStarted.Broadcast(CountdownEndTime);
	}
	UFUNCTION()
	void BroadcastAttributeChanged(AActor* AffectedActor, EAttribute Attribute, float NewValue, float Delta)
	{
		DEBUG_LOG("EventBus: BroadcastAttributeChanged() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Attribute, NewValue, Delta);
		OnAttributeChanged.Broadcast(AffectedActor, Attribute, NewValue, Delta);
	}
	UFUNCTION()
	void BroadcastCharacterDied(AActor* DeadActor)
	{
		DEBUG_LOG("EventBus: BroadcastCharacterDied() - Dead Actor: %s", *DeadActor->GetName());
		OnCharacterDied.Broadcast(DeadActor);
	}

};
