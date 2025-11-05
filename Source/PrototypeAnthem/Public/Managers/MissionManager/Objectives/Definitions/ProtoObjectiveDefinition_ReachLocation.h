#pragma once
#include "CoreMinimal.h"
#include "ProtoObjectiveDefinition.h"
#include "ProtoObjectiveDefinition_ReachLocation.generated.h"


UCLASS(BlueprintType)
class PROTOTYPEANTHEM_API UProtoObjectiveDefinition_ReachLocation : public UProtoObjectiveDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ReachLocation")
	float TargetLocationRadius;
};
