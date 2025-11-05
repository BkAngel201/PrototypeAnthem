// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MissionManager/Objectives/ProtoObjectiveBase.h"


#include "EventHub/ProtoEventHub.h"
#include "Debug/ProtoDebug.h"
#include "Managers/MissionManager/ProtoMissionManager.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition.h"

void UProtoObjectiveBase::InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def, UProtoMissionManager* Owner_MM)
{
	PROTO_NULL_CHECK(Def, "Objective Definition");
	PROTO_NULL_CHECK(Owner_MM, "Mission Manager");
	
	PROTO_OBJECTIVE_LOG("[%s] ⚠️ InitializeFromDefinition(Base)", *GetClass()->GetName());
	if (bInitialized)
	{
		PROTO_OBJECTIVE_LOG("[%s] ⚠️ Objective already initialized", *GetClass()->GetName());
		return;
	}
	bInitialized = true;
	ObjectiveName = Def->ObjectiveName;
	ObjectiveDescription = Def->ObjectiveDescription;
	ObjectiveType = Def->ObjectiveType;
	ObjectiveIndex = Index;
	Owner_MissionManager = Owner_MM;
	SetObjectiveState(EObjectiveState::Inactive);
}

void UProtoObjectiveBase::SetObjectiveState(EObjectiveState NewState)
{
	PROTO_NULL_CHECK(Owner_MissionManager, "Mission Manager")
	if (ObjectiveState == NewState)
	{
		PROTO_OBJECTIVE_LOG("[%s] Cannot change state, new state equal to previous state", *GetClass()->GetName());
		return;
	}
	PROTO_STATE_CHANGE_ENUM("ObjectiveState", ObjectiveState, NewState);
	ObjectiveState = NewState;
	
	PROTO_AUTH_CHECK("SetObjectiveState");
	Owner_MissionManager->HandleObjectiveStateChanged(this, NewState); // (MissionManager -> ObjectiveManager -> ObjectiveBase
}

void UProtoObjectiveBase::SetObjectiveDataUpdated(TArray<FObjectiveDataPair> DataUpdated)
{
	PROTO_AUTH_CHECK("SetObjectiveDataUpdated");
	PROTO_NULL_CHECK(Owner_MissionManager, "Mission Manager")
	for (FObjectiveDataPair newData : DataUpdated)
	{
		PROTO_OBJECTIVE_LOG("[%s] data updated: %s = %.1f", *GetClass()->GetName(), *newData.Label, newData.Value);
	} 

	Owner_MissionManager->HandleObjectiveDataUpdate(this, DataUpdated);
}

void UProtoObjectiveBase::StartObjective_Implementation()
{
	PROTO_AUTH_CHECK("StartObjective");
	// Early exit if already active/failed
	if (ObjectiveState == EObjectiveState::Active || ObjectiveState == EObjectiveState::Failed)
	{
		PROTO_OBJECTIVE_LOG("[%s] Cannot start - already in state %d", *GetClass()->GetName(), (uint8)ObjectiveState);
		return;
	}
	SetObjectiveState(EObjectiveState::Active);
	OnObjectiveStarted();
	PROTO_OBJECTIVE_LOG("[%s] '%s' started", *GetClass()->GetName(), *ObjectiveName.ToString());
}

void UProtoObjectiveBase::CompleteObjective_Implementation()
{
	PROTO_AUTH_CHECK("CompleteObjective");
	// Early exit if already completed/failed
	if (ObjectiveState == EObjectiveState::Completed || ObjectiveState == EObjectiveState::Failed)
	{
		PROTO_OBJECTIVE_LOG("[%s] Cannot complete - already in state %d", *GetClass()->GetName(), (uint8)ObjectiveState);
		return;
	}
	SetObjectiveState(EObjectiveState::Completed);
 	OnObjectiveCompleted();
	PROTO_OBJECTIVE_LOG("[%s] '%s' completed", *GetClass()->GetName(), *ObjectiveName.ToString());
}

void UProtoObjectiveBase::FailObjective_Implementation()
{
	PROTO_AUTH_CHECK("FailObjective");
	// Early exit if already failed
	if (ObjectiveState == EObjectiveState::Failed)
	{
		PROTO_OBJECTIVE_LOG("[%s] Cannot fail - already in state %d", *GetClass()->GetName(), (uint8)ObjectiveState);
		return;
	}
	SetObjectiveState(EObjectiveState::Failed);
	OnObjectiveFailed();
	PROTO_OBJECTIVE_LOG("[%s] '%s' failed", *GetClass()->GetName(), *ObjectiveName.ToString());
}

bool UProtoObjectiveBase::HasAuthority()
{
	if (!Owner_MissionManager)
	{
		PROTO_ERROR("[%s] No MissionManager - cannot check authority", *GetClass()->GetName());
		return false;
	}
    
	const bool bHasAuth = Owner_MissionManager->HasAuthority();
    
	if (!bHasAuth)
	{
		PROTO_WARN("[%s] Authority check failed - running on client", *GetClass()->GetName());
	}
    
	return bHasAuth;
}

UProtoEventHub* UProtoObjectiveBase::GetEventHub() const
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			return GI->GetSubsystem<UProtoEventHub>();
		}
	}
	return nullptr;
}

