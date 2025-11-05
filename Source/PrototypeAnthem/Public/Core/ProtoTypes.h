#pragma once

#include "CoreMinimal.h"
#include "ProtoTypes.generated.h"

class UProtoObjectiveBase;


UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Health UMETA(DisplayName="Health", ToolTip="Health points"),
	Shield UMETA(DisplayName="Shield", ToolTip="Shield points"),
};


UENUM(BlueprintType)
enum class ERoleType : uint8
{
	None      UMETA(DisplayName="None", ToolTip="Unassigned or spectator role"),
	Tank      UMETA(DisplayName="Tank", ToolTip="High durability frontline"),
	DPS       UMETA(DisplayName="DPS", ToolTip="Damage dealer role"),
	Utility   UMETA(DisplayName="Utility", ToolTip="Support or control role")
};

UCLASS()
class UProtoTypeHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns a readable mission state string */
	UFUNCTION(BlueprintPure, Category = "Proto|Helpers")
	static FText GetMissionStateText(EMissionState State);

	/** Returns a readable role name string */
	UFUNCTION(BlueprintPure, Category = "Proto|Helpers")
	static FText GetRoleTypeText(ERoleType Role);

	/** Returns a readable attribute name string */
	UFUNCTION(BlueprintPure, Category = "Proto|Helpers")
	static FText GetAttributeText(EAttribute Attribute);

	/** Returns a readable objective state string */
	UFUNCTION(BlueprintPure, Category = "Proto|Helpers")
	static FText GetObjectiveStateText(EObjectiveState State);

	UFUNCTION(BlueprintPure, Category = "Proto|Helpers|AssetManager")
	static FName GetAssetType_Widget() { return TEXT("WidgetDefinition"); }

	UFUNCTION(BlueprintPure, Category = "Proto|Helpers|AssetManager")
	static FName GetAssetType_Mission() { return TEXT("MissionDefinition"); }

	UFUNCTION(BlueprintPure, Category = "Proto|Helpers|AssetManager")
	static FName GetAssetType_Objective() { return TEXT("ObjectiveDefinition"); }
};