// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoObjectiveBase.generated.h"

struct FObjectiveDataPair;
enum class EObjectiveType : uint8;
class UProtoEventHub;
class UProtoMissionManager;
class AProtoMission_GM;
enum class EMissionState : uint8;
class UProtoObjectiveDefinition;
/**
 * 
 */

UCLASS()
class PROTOTYPEANTHEM_API UProtoObjectiveBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category="Objective", meta=(AllowPrivateAccess="true"))
	bool bInitialized = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	FText ObjectiveName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	EObjectiveType ObjectiveType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	FText ObjectiveDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	EObjectiveState ObjectiveState;
	
	UPROPERTY(VisibleAnywhere, Category="Objective")
	int32 ObjectiveIndex;

	UPROPERTY()
	AProtoMission_GM* Owning_GM;
	UPROPERTY()
	UProtoMissionManager* Owner_MissionManager;

	UFUNCTION()
	virtual void InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def,UProtoMissionManager* Owner_MM);

	UFUNCTION(BlueprintImplementableEvent)
	void OnObjectiveStarted();
	UFUNCTION(BlueprintImplementableEvent)
	void OnObjectiveCompleted();
	UFUNCTION(BlueprintImplementableEvent)
	void OnObjectiveFailed();
	
	

	UFUNCTION(BlueprintCallable)
	void SetObjectiveState(EObjectiveState NewState);

	UFUNCTION(BlueprintCallable)
	void SetObjectiveDataUpdated(TArray<FObjectiveDataPair> DataUpdated);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Objective")
	void StartObjective();
	virtual void StartObjective_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Objective")
	void CompleteObjective();
	virtual void CompleteObjective_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Objective")
	void FailObjective();
	virtual void FailObjective_Implementation();
	
	bool HasAuthority();
	UProtoEventHub* GetEventHub() const;
};
