// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/MissionManager/Objectives/ProtoObjectiveBase.h"
#include "ProtoObjective_ReachLocation.generated.h"

class UProtoObjectiveDefinition_ReachLocation;
class AProtoMission_TargetLocation;
/**
 * 
 */

UCLASS()
class PROTOTYPEANTHEM_API UProtoObjective_ReachLocation : public UProtoObjectiveBase
{
	GENERATED_BODY()

public:
	UProtoObjective_ReachLocation();
	virtual void InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def, UProtoMissionManager* Owner_MM) override;

	UPROPERTY(VisibleAnywhere, Category="Objective")
	UProtoObjectiveDefinition_ReachLocation* ObjectiveDefinition; // from the DataAsset
	
	UPROPERTY(EditAnywhere)
	float Radius;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProtoMission_TargetLocation> TargetActorClass;

	UPROPERTY(EditAnywhere)
	AProtoMission_TargetLocation* TargetActor;

	void StartObjective_Implementation();
	UFUNCTION()
	void OnTargetReached(AActor* Player);
};
