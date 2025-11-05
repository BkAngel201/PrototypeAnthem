#pragma once

#include "CoreMinimal.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition.h"
#include "ProtoMissionDefinition.generated.h"

class UProtoMissionDefinition;
class UProtoObjectiveDefinition;

UENUM(BlueprintType)
enum class EMissionState : uint8
{
	Inactive   UMETA(DisplayName = "Inactive",  ToolTip = "Mission system is idle or not yet initialized."),
	Countdown  UMETA(DisplayName = "Countdown", ToolTip = "Mission is preparing to start. Players see a countdown before activation."),
	Active     UMETA(DisplayName = "Active",    ToolTip = "Mission is currently running. Objectives are being tracked and updated."),
	Completed  UMETA(DisplayName = "Completed", ToolTip = "All objectives are finished and the mission has succeeded."),
	Failed     UMETA(DisplayName = "Failed",    ToolTip = "Mission ended unsuccessfully or was aborted before completion.")
};

UENUM(BlueprintType)
enum class EMissionType : uint8
{
	Main        UMETA(DisplayName = "Main",  ToolTip = "Primary mission objectives that must be completed to succeed."),
	Side        UMETA(DisplayName = "Side",  ToolTip = "Optional objectives that provide additional rewards but aren't required."),
	Defense     UMETA(DisplayName = "Defense",  ToolTip = "Objectives focused on protecting assets or positions from enemy attacks."),
	Extraction  UMETA(DisplayName = "Extraction",  ToolTip = "Objectives related to safely exiting the mission area after completion."),

};

USTRUCT(BlueprintType)
struct FMissionReward
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Credits = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 XP = 0;
};

USTRUCT(BlueprintType)
struct FMissionRuntimeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UProtoMissionDefinition* MissionDefinition = nullptr; // reference to static data

	UPROPERTY(BlueprintReadOnly)
	EMissionState MissionState = EMissionState::Inactive;

	UPROPERTY(BlueprintReadOnly)
	TArray<FObjectiveRuntimeData> Objectives; // per-objective state
	
};

UCLASS(EditInlineNew, BlueprintType)
class UProtoMissionDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	FText MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	FText MissionDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	EMissionType MissionType = EMissionType::Main;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	TArray<UProtoObjectiveDefinition*> Objectives;   // one per step (expand later to array-of-arrays)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	FMissionReward Reward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
	UTexture2D* MissionThumbnail = nullptr;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// Tells AssetManager this is a MissionDefinition asset
		return FPrimaryAssetId(TEXT("MissionDefinition"), GetFName());
	}
	
};
