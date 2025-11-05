#pragma once
#include "CoreMinimal.h"
#include "ProtoObjectiveDefinition.h"
#include "ProtoObjectiveDefinition_Interact.generated.h"


UCLASS(BlueprintType)
class PROTOTYPEANTHEM_API UProtoObjectiveDefinition_Interact : public UProtoObjectiveDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact")
	FName InteractionTag;
};
