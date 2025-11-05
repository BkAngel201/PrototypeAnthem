#include "Core/ProtoTypes.h"

#include "Managers/MissionManager/Missions/ProtoMissionDefinition.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition.h"

FText UProtoTypeHelpers::GetMissionStateText(EMissionState State)
{
	switch (State)
	{
	case EMissionState::Countdown: return FText::FromString(TEXT("Countdown"));
	case EMissionState::Active:    return FText::FromString(TEXT("Active"));
	case EMissionState::Completed: return FText::FromString(TEXT("Completed"));
	case EMissionState::Failed:    return FText::FromString(TEXT("Failed"));
	default:                       return FText::FromString(TEXT("Inactive"));
	}
}

FText UProtoTypeHelpers::GetRoleTypeText(ERoleType Role)
{
	switch (Role)
	{
	case ERoleType::Tank:    return FText::FromString(TEXT("Tank"));
	case ERoleType::DPS:     return FText::FromString(TEXT("DPS"));
	case ERoleType::Utility: return FText::FromString(TEXT("Utility"));
	default:                 return FText::FromString(TEXT("None"));
	}
}

FText UProtoTypeHelpers::GetAttributeText(EAttribute Attribute)
{
	switch (Attribute)
	{
		case EAttribute::Health: return FText::FromString(TEXT("Health"));
		case EAttribute::Shield: return FText::FromString(TEXT("Shield"));
		default:                 return FText::FromString(TEXT("None"));
	}
}

FText UProtoTypeHelpers::GetObjectiveStateText(EObjectiveState State)
{
	switch (State)
	{
		case EObjectiveState::Inactive: return FText::FromString(TEXT("Inactive"));
		case EObjectiveState::Active:   return FText::FromString(TEXT("Active"));
		case EObjectiveState::Completed: return FText::FromString(TEXT("Completed"));
		case EObjectiveState::Failed:   return FText::FromString(TEXT("Failed"));
		default:                       return FText::FromString(TEXT("Unknown"));
	}
}
