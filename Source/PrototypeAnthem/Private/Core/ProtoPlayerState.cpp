// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProtoPlayerState.h"

#include "Debug/ProtoDebug.h"
#include "Net/UnrealNetwork.h"

AProtoPlayerState::AProtoPlayerState()
{
	bReplicates = true;
	bIsReady = false;
	SelectedRole = ERole_Type::Tank;
}

void AProtoPlayerState::OnRep_SelectedRole()
{
	NET_LOG("OnRep_SelectedRole");
	UE_LOG(LogProto, Warning, TEXT("OnRep_SelectedRole: %d"), (uint8)SelectedRole);
	OnLobbyDataChanged.Broadcast(this);
}

void AProtoPlayerState::OnRep_IsReady()
{
	NET_LOG("OnRep_IsReady");
	UE_LOG(LogProto, Warning, TEXT("OnRep_IsReady: %s"), bIsReady ? TEXT("true") : TEXT("false"));
	OnLobbyDataChanged.Broadcast(this);
}

void AProtoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProtoPlayerState, SelectedRole);
	DOREPLIFETIME(AProtoPlayerState, bIsReady);
}
