// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoMissionManager.generated.h"

struct FObjectiveDataPair;
enum class EObjectiveState : uint8;
class UProtoEventHub;
class UProtoMissionDefinition;
class UProtoObjectiveDefinition;
class UProtoObjectiveBase;
class AProtoGameState;
enum class EMissionState: uint8;
/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class PROTOTYPEANTHEM_API UProtoMissionManager : public UObject
{
	GENERATED_BODY()
public:
	UProtoMissionManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	UProtoMissionDefinition* CurrentMissionDefinition;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mission")
	TArray<UProtoObjectiveBase*> ActiveObjectives;

	UFUNCTION()
	void HandleObjectiveStateChanged(UProtoObjectiveBase* Objective, EObjectiveState NewState);

	UFUNCTION()
	void HandleObjectiveDataUpdate(UProtoObjectiveBase* Objective, TArray<FObjectiveDataPair> DataUpdated);

	
	UFUNCTION()
	void Initialize(AProtoGameState* InMissionGM);

	UFUNCTION()
	void StartObjectives();
	
	UFUNCTION(BlueprintCallable, Category="Mission")
	void StartMission();
	
	UFUNCTION()
	void SetMissionState(EMissionState NewState);
	UFUNCTION()
	void StartMissionCountdown();
	UFUNCTION()
	void ActivateMission();
	UFUNCTION()
	void CompleteMission();
	UFUNCTION()
	void FailMission();
	
	void ClearActiveObjectives();
	bool HasAuthority();
	UProtoEventHub* GetEventHub() const;

	

private:
	UPROPERTY()
	AProtoGameState* Owner_GS;
	
	FTimerHandle CountdownHandle;
	FTimerHandle ActiveHandle;
	FTimerHandle TravelHandle;
};
