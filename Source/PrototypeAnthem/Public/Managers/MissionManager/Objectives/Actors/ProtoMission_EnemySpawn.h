// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProtoMission_EnemySpawn.generated.h"

class UProtoObjectiveDefinition_KillEnemies;

UCLASS()
class PROTOTYPEANTHEM_API AProtoMission_EnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AProtoMission_EnemySpawn();

protected:
	

public:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitFromDefinition(UProtoObjectiveDefinition_KillEnemies* Definition);

	

};
