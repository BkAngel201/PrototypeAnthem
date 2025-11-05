// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ProtoGameState.h"

#include "Managers/MissionManager/ProtoMissionManager.h"
#include "Debug/ProtoDebug.h"
#include "Net/UnrealNetwork.h"
#include "EventHub/ProtoEventHub.h"
#include "Managers/WidgetManager/ProtoWidgetManager.h"

AProtoGameState::AProtoGameState()
{
}

void AProtoGameState::BeginPlay()
{
	Super::BeginPlay();
	FString MapName = GetWorld()->GetMapName();

	if (MapName.Contains(TEXT("Mission")))
	{
		GameContext = EGameContext::Mission;
	}
	else if (MapName.Contains(TEXT("Lobby")))
	{
		GameContext = EGameContext::Lobby;
	}
	else
	{
		GameContext = EGameContext::Other;
	}
	
	if (HasAuthority() && GameContext == EGameContext::Mission)
	{
		MissionManager = NewObject<UProtoMissionManager>(this, MissionManagerClass);
		MissionManager->Initialize(this);  // optional
		MissionManager->StartMission();
	}
}

void AProtoGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	NET_LOG("PlayerArray changed (Player Added)");
	OnPlayerArrayChanged.Broadcast(this);
}

void AProtoGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	NET_LOG("PlayerArray changed (Player Removed)");
	OnPlayerArrayChanged.Broadcast(this);
}

void AProtoGameState::OnRep_CountdownEndTime()
{
	NET_LOG("OnRep_CountdownTime");
	DEBUG_LOG("OnRep_CountdownTime: %f", CountdownEndTime);
	if (UProtoEventHub* Hub = GetGameInstance()->GetSubsystem<UProtoEventHub>())
	{
		Hub->BroadcastScreenCountdownStarted(CountdownEndTime);
	}
}

void AProtoGameState::OnRep_MissionData()
{
	DEBUG_LOG("GameState: OnRep_MissionState() - MissionState Changed");
	if (UProtoEventHub* Hub = GetGameInstance()->GetSubsystem<UProtoEventHub>())
	{
		Hub->BroadcastMissionUpdated(CurrentMissionData);
	}
}

void AProtoGameState::OnRep_GameContext()
{
	DEBUG_LOG("🟢 GameContext replicated: %s", *UEnum::GetValueAsString(GameContext));

	if (UProtoWidgetManager* WidgetManager = GetGameInstance()->GetSubsystem<UProtoWidgetManager>())
	{
		WidgetManager->ShowScreenWidget();
	}
}

void AProtoGameState::UpdateMissionDataSnapshot(const FMissionRuntimeData& MissionData)
{
	DEBUG_LOG("GameState: UpdateMissionDataSnapshot() MissionData Updated");
	CurrentMissionData = MissionData; // replicated
	ForceNetUpdate();
	OnRep_MissionData();
}

UProtoEventHub* AProtoGameState::GetEventHub() const
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

void AProtoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProtoGameState, CountdownEndTime);
	DOREPLIFETIME(AProtoGameState, CurrentMissionData);
}
