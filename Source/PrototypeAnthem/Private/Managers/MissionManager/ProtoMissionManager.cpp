// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MissionManager/ProtoMissionManager.h"

#include "Game/ProtoGameState.h"
#include "Game/GameModes/ProtoMission_GM.h"
#include "EventHub/ProtoEventHub.h"
#include "Debug/ProtoDebug.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/MissionManager/Missions/ProtoMissionDefinition.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition.h"
#include "Managers/MissionManager/Objectives/ProtoObjective_ReachLocation.h"

UProtoMissionManager::UProtoMissionManager()
{
}

void UProtoMissionManager::Initialize(AProtoGameState* InMissionGS)
{
	PROTO_NULL_CHECK(InMissionGS, "Owner GameState");
	PROTO_NULL_CHECK(CurrentMissionDefinition, "Current mission Definition Data");
	Owner_GS = InMissionGS;
	
	PROTO_AUTH_CHECK("Initialize");
	PROTO_MISSION_LOG("🚀 Initializing mission: %s", *CurrentMissionDefinition->MissionName.ToString());
	
	AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>();
	PROTO_NULL_CHECK(GS, "GameState");
	
	FMissionRuntimeData Data = GS->CurrentMissionData;
	Data.MissionDefinition = CurrentMissionDefinition;
	Data.Objectives.Empty();

	for (UProtoObjectiveDefinition* Definition : CurrentMissionDefinition->Objectives)
	{
		FObjectiveRuntimeData ObjData;
		ObjData.Definition = Definition;
		ObjData.State = EObjectiveState::Inactive;
		Data.Objectives.Add(ObjData);
	}
	GS->UpdateMissionDataSnapshot(Data);
	PROTO_SUCCESS("Mission initialized with %d objectives", Data.Objectives.Num());
}

void UProtoMissionManager::SetMissionState(EMissionState NewState)
{
	PROTO_AUTH_CHECK("SetMissionState");
	
	// Update runtime data and notify GameState
	AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>();
	PROTO_NULL_CHECK(GS, "GameState");
	
	FMissionRuntimeData Data = GS->CurrentMissionData;
	const EMissionState OldState = Data.MissionState;
	
	PROTO_STATE_CHANGE_ENUM("MissionState", OldState, NewState);
	Data.MissionState = NewState;
	GS->UpdateMissionDataSnapshot(Data);
}

void UProtoMissionManager::StartMission()
{
	PROTO_AUTH_CHECK("StartMission");
	PROTO_MISSION_LOG("🚀 Starting Mission: %s", *GetNameSafe(CurrentMissionDefinition));
	StartMissionCountdown();
}

void UProtoMissionManager::StartMissionCountdown()
{
	PROTO_AUTH_CHECK("StartMissionCountdown");
	PROTO_NULL_CHECK(Owner_GS, "Owner_GS");
	
	SetMissionState(EMissionState::Countdown);
	
	const float CountdownDuration = 2.0f;
	Owner_GS->GetWorldTimerManager().SetTimer(CountdownHandle, this, &UProtoMissionManager::ActivateMission, CountdownDuration, false);

	const bool bTimerActive = Owner_GS->GetWorldTimerManager().IsTimerActive(CountdownHandle);
	PROTO_TIMER_LOG("MissionCountdown", CountdownDuration, bTimerActive);
}

void UProtoMissionManager::ActivateMission()
{
	PROTO_AUTH_CHECK("ActivateMission");
	SetMissionState(EMissionState::Active);
	StartObjectives();
}

void UProtoMissionManager::StartObjectives()
{
	PROTO_AUTH_CHECK("StartObjectives");
	PROTO_NULL_CHECK(CurrentMissionDefinition, "Current Mission Definition Data");

	ClearActiveObjectives();

	int32 IndexCount = 0;
	for (UProtoObjectiveDefinition* Definition : CurrentMissionDefinition->Objectives)
	{
		if (!Definition || !Definition->ObjectiveClass) continue;
		auto* Objective = NewObject<UProtoObjectiveBase>(this, Definition->ObjectiveClass);
		Objective->InitializeFromDefinition(IndexCount, Definition, this);
		ActiveObjectives.Add(Objective);
		if (IndexCount == 0)
		{
			Objective->StartObjective();
		}
		IndexCount++;
	}
	PROTO_OBJECTIVE_LOG("🚀 Started %d of %d objectives from %s",
			  ActiveObjectives.Num(),
			  CurrentMissionDefinition->Objectives.Num(),
			  *CurrentMissionDefinition->MissionName.ToString());
}

void UProtoMissionManager::HandleObjectiveStateChanged(UProtoObjectiveBase* Objective, EObjectiveState NewState)
{
	PROTO_NULL_CHECK(Objective, "Objective");
	// Update runtime data and notify GameState
	AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>();
	PROTO_NULL_CHECK(GS, "GameState");
	
	FMissionRuntimeData Data = GS->CurrentMissionData;
	const EObjectiveState OldState = Data.Objectives[Objective->ObjectiveIndex].State;
	PROTO_OBJECTIVE_LOG("Objective '%s' [%d] changed state", 
						*Objective->ObjectiveName.ToString(), 
						Objective->ObjectiveIndex);
	PROTO_STATE_CHANGE_ENUM("ObjectiveState", OldState, NewState);
	
	Data.Objectives[Objective->ObjectiveIndex].State = NewState;
	GS->UpdateMissionDataSnapshot(Data);
	
	
	if (NewState == EObjectiveState::Completed)
	{
		if (Objective->ObjectiveIndex == CurrentMissionDefinition->Objectives.Num() - 1)
		{
			PROTO_MISSION_LOG("✅ All objectives completed - finishing mission");
			CompleteMission();
		} else
		{
			int32 NextIndex = Objective->ObjectiveIndex + 1;
			PROTO_OBJECTIVE_LOG("🚀 Starting next objective [%d]", NextIndex);
			ActiveObjectives[NextIndex]->StartObjective();
		}
	}
}

void UProtoMissionManager::HandleObjectiveDataUpdate(UProtoObjectiveBase* Objective, TArray<FObjectiveDataPair> DataUpdated)
{
	PROTO_NULL_CHECK(Objective, "Objective");

	AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>();
	PROTO_NULL_CHECK(GS, "GameState");
	
	FMissionRuntimeData Data = GS->CurrentMissionData;
	Data.Objectives[Objective->ObjectiveIndex].CustomData = DataUpdated;
	for (FObjectiveDataPair newData : DataUpdated)
	{
		PROTO_OBJECTIVE_LOG("'%s' data updated: %s = %.1f", *Objective->ObjectiveName.ToString(), *newData.Label, newData.Value);
	} 
	GS->UpdateMissionDataSnapshot(Data);
}

void UProtoMissionManager::CompleteMission()
{
	PROTO_AUTH_CHECK("CompleteMission");
	PROTO_NULL_CHECK(Owner_GS, "Owner_GS");
	
	SetMissionState(EMissionState::Completed);
	PROTO_SUCCESS("✅ Mission Completed: %s", *CurrentMissionDefinition->MissionName.ToString());

	AProtoMission_GM* GM = Cast<AProtoMission_GM>(UGameplayStatics::GetGameMode(Owner_GS->GetWorld()));
	PROTO_NULL_CHECK(GM, "Game Mode");

	const float TravelDelay  = 2.0f;
	Owner_GS->GetWorldTimerManager().SetTimer(TravelHandle, GM, &AProtoMission_GM::TravelToLobby, TravelDelay , false);
	const bool bTimerActive = Owner_GS->GetWorldTimerManager().IsTimerActive(TravelHandle);
	PROTO_TIMER_LOG("TravelToLobby", TravelDelay , bTimerActive);
	
	ClearActiveObjectives();
}

void UProtoMissionManager::FailMission()
{
	PROTO_AUTH_CHECK("FailMission");
	PROTO_MISSION_LOG("💀 Mission Failed: %s", *CurrentMissionDefinition->MissionName.ToString());
	
	SetMissionState(EMissionState::Failed);
	ClearActiveObjectives();
}

void UProtoMissionManager:: ClearActiveObjectives()
{
	for (UProtoObjectiveBase* Obj : ActiveObjectives)
	{
		if (Obj)
		{
			Obj->Owner_MissionManager = nullptr;
		}
	}
	ActiveObjectives.Empty();
}

bool UProtoMissionManager::HasAuthority()
{
	if (!Owner_GS)
	{
		PROTO_ERROR("[%s] No Game State - cannot check authority", *GetClass()->GetName());
		return false;
	}
    
	const bool bHasAuth = Owner_GS->HasAuthority();
    
	if (!bHasAuth)
	{
		PROTO_WARN("[%s] Authority check failed - running on client", *GetClass()->GetName());
	}
    
	return bHasAuth;
}

UProtoEventHub* UProtoMissionManager::GetEventHub() const
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
