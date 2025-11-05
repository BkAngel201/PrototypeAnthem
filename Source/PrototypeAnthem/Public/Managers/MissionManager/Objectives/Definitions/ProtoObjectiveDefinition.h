#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProtoObjectiveDefinition.generated.h"

class UProtoObjectiveBase;

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	ReachLocation UMETA(DisplayName="Reach Location", ToolTip="Reach a specific location"),
	Interact      UMETA(DisplayName="Interact", ToolTip="Interact with an object"),
	Eliminate     UMETA(DisplayName="Eliminate Enemies", ToolTip="Eliminate all enemies"),
};

UENUM(BlueprintType)
enum class EObjectiveState : uint8
{
	Inactive   UMETA(DisplayName = "Inactive",  ToolTip = "Objective is not yet initialized."),
	Active     UMETA(DisplayName = "Active",    ToolTip = "Objective is currently running. Objectives are being tracked and updated."),
	Completed  UMETA(DisplayName = "Completed", ToolTip = "Objective is finished and the mission has succeeded."),
	Failed     UMETA(DisplayName = "Failed",    ToolTip = "Objective ended unsuccessfully or was aborted before completion.")
};

USTRUCT(BlueprintType)
struct FObjectiveDataPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag KeyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Label;
};

USTRUCT(BlueprintType)
struct FObjectiveRuntimeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UProtoObjectiveDefinition* Definition = nullptr; // reference to static data

	UPROPERTY(BlueprintReadOnly)
	EObjectiveState State = EObjectiveState::Inactive;

	UPROPERTY(BlueprintReadOnly)
	TArray<FObjectiveDataPair> CustomData;
};

UCLASS(Abstract, EditInlineNew, BlueprintType)
class UProtoObjectiveDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UProtoObjectiveBase> ObjectiveClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform TargetTransform;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// Tells AssetManager this is an ObjectiveDefinition asset
		return FPrimaryAssetId(TEXT("ObjectiveDefinition"), GetFName());
	}
};
