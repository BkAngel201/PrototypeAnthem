// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ProtoPlayerState.h"

#include "Game/ProtoGameState.h"
#include "Core/ProtoTypes.h"
#include "Debug/ProtoDebug.h"
#include "Net/UnrealNetwork.h"

AProtoPlayerState::AProtoPlayerState()
{
	bReplicates = true;
	bIsReady = false;
	SelectedRole = ERoleType::Tank;
}

void AProtoPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		if (AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>())
		{
			GS->OnPlayerArrayChanged.Broadcast(GS);
		}
	}
}

void AProtoPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	OnLobbyDataChanged.Broadcast(this);
}

void AProtoPlayerState::OnRep_SelectedRole()
{
	NET_LOG("OnRep_SelectedRole");
	DEBUG_LOG("OnRep_SelectedRole: %d", (uint8)SelectedRole);
	OnLobbyDataChanged.Broadcast(this);
}

void AProtoPlayerState::OnRep_IsReady()
{
	NET_LOG("OnRep_IsReady");
	DEBUG_LOG("OnRep_IsReady: %s", bIsReady ? TEXT("true") : TEXT("false"));
	OnLobbyDataChanged.Broadcast(this);
}

void AProtoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProtoPlayerState, SelectedRole);
	DOREPLIFETIME(AProtoPlayerState, bIsReady);
}
