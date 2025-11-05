#pragma once
#include "CoreMinimal.h"
#include "ProtoObjectiveDefinition.h"
#include "ProtoObjectiveDefinition_KillEnemies.generated.h"

USTRUCT(BlueprintType)
struct FKillEnemiesRequired
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eliminate")
	TSubclassOf<AActor> TargetEnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eliminate")
	int32 RequiredKills = 5;
};

UCLASS(BlueprintType)
class PROTOTYPEANTHEM_API UProtoObjectiveDefinition_KillEnemies : public UProtoObjectiveDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eliminate")
	TArray<FKillEnemiesRequired> EnemiesRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eliminate")
	float SpawnRadius;
};
