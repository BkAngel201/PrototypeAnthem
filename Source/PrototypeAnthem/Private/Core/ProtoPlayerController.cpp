// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProtoPlayerController.h"

#include "Core/ProtoGameMode.h"
#include "Core/ProtoPlayerState.h"
#include "Debug/ProtoDebug.h"

AProtoPlayerController::AProtoPlayerController()
{
}
void AProtoPlayerController::Server_SelectRole_Implementation(ERole_Type InRole)
{
	NET_LOG("Server_SelectRole");
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		UE_LOG(LogProto, Warning, TEXT("Server_SelectRole called: %d"), (uint8)InRole);
		PS->SelectedRole = InRole;
		PS->OnLobbyDataChanged.Broadcast(PS);
	}
}

void AProtoPlayerController::Server_SetReady_Implementation(bool bInReady)
{
	NET_LOG("Server_SetReady");
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		UE_LOG(LogProto, Warning, TEXT("Server_SetReady called: %s"), bInReady ? TEXT("true") : TEXT("false"));
		PS->bIsReady = bInReady;
		PS->OnLobbyDataChanged.Broadcast(PS);

		if (HasAuthority())
		{
			if (AProtoGameMode* GM = GetWorld()->GetAuthGameMode<AProtoGameMode>())
			{
				GM->CheckAllPlayersReady();
			}
		}
	}
}

void AProtoPlayerController::SelectRole(ERole_Type NewRole)
{
	UE_LOG(LogProto, Warning, TEXT("Client calling SelectRole: %d"), (uint8)NewRole);
	Server_SelectRole(NewRole);
}

void AProtoPlayerController::SetReady(bool bNewReady)
{
	UE_LOG(LogProto, Warning, TEXT("Client calling SetReady: %s"), bNewReady ? TEXT("true") : TEXT("false"));
	Server_SetReady(bNewReady);
}

void AProtoPlayerController::ToggleReady()
{
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		const bool bNewReady = !PS->bIsReady;
		UE_LOG(LogProto, Warning, TEXT("Toggling Ready → %s"), bNewReady ? TEXT("true") : TEXT("false"));
		Server_SetReady(bNewReady);
	}
}
