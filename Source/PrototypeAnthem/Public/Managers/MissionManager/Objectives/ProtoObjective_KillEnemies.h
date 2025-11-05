// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/MissionManager/Objectives/ProtoObjectiveBase.h"
#include "ProtoObjective_KillEnemies.generated.h"

class AProtoMission_EnemySpawn;
class UProtoObjectiveDefinition_KillEnemies;
/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API UProtoObjective_KillEnemies : public UProtoObjectiveBase
{
	GENERATED_BODY()
public:
	UProtoObjective_KillEnemies();
	virtual void InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def, UProtoMissionManager* Owner_MM) override;

	UPROPERTY(VisibleAnywhere, Category="Objective")
	UProtoObjectiveDefinition_KillEnemies* ObjectiveDefinition; // from the DataAsset

	UPROPERTY(VisibleAnywhere, Category="Objective|Kill")
	int32 CurrentKills = 0;

	UPROPERTY(VisibleAnywhere, Category="Objective|Kill")
	int32 RequiredKills = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProtoMission_EnemySpawn> TargetActorClass;

	UPROPERTY(EditAnywhere)
	AProtoMission_EnemySpawn* TargetActor;

	UFUNCTION()
	void HandleOnCharacterDie(AActor* DeadActor);
	void StartObjective_Implementation();
	void FailObjective_Implementation();
	void SetObjectiveCustomData();
};
