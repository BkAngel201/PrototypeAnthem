// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ProtoPlayerController.h"

#include "Player/ProtoPlayerState.h"
#include "Game/GameModes/ProtoLobby_GM.h"
#include "Debug/ProtoDebug.h"
#include "Managers/WidgetManager/ProtoWidgetManager.h"


AProtoPlayerController::AProtoPlayerController()
{
}

void AProtoPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return; // Don't create UI on server or remote clients
	}
	// Ensure widgets show after one tick
	GetWorldTimerManager().SetTimerForNextTick(this, &AProtoPlayerController::DeferredShowWidgets);
	// 🔁 Safety fallback after 1 second (replication / travel delay)
	GetWorldTimerManager().SetTimer(ContextCheckHandle, this, &AProtoPlayerController::DeferredShowWidgets, 1.0f, false);
}

void AProtoPlayerController::DeferredShowWidgets()
{
	if (UProtoWidgetManager* WidgetManager = GetGameInstance()->GetSubsystem<UProtoWidgetManager>())
	{
		WidgetManager->SetCachedOwner(this);
		WidgetManager->ShowScreenWidget();
	}
}

void AProtoPlayerController::Server_SelectRole_Implementation(ERoleType InRole)
{
	NET_LOG("Server_SelectRole");
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		DEBUG_LOG("Server_SelectRole called: %d", (uint8)InRole);
		PS->SelectedRole = InRole;
		PS->OnLobbyDataChanged.Broadcast(PS);
	}
}

void AProtoPlayerController::Server_SetReady_Implementation(bool bInReady)
{
	NET_LOG("Server_SetReady");
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		DEBUG_LOG("Server_SetReady called: %s", bInReady ? TEXT("true") : TEXT("false"));
		PS->bIsReady = bInReady;
		PS->OnLobbyDataChanged.Broadcast(PS);

		if (HasAuthority())
		{
			if (AProtoLobby_GM* GM = GetWorld()->GetAuthGameMode<AProtoLobby_GM>())
			{
				GM->CheckAllPlayersReady();
			}
		}
	}
}


void AProtoPlayerController::SelectRole(ERoleType NewRole)
{
	DEBUG_LOG("Client calling SelectRole: %d", (uint8)NewRole);
	Server_SelectRole(NewRole);
}

void AProtoPlayerController::SetReady(bool bNewReady)
{
	DEBUG_LOG("Client calling SetReady: %s", bNewReady ? TEXT("true") : TEXT("false"));
	Server_SetReady(bNewReady);
}

void AProtoPlayerController::ToggleReady()
{
	if (AProtoPlayerState* PS = GetPlayerState<AProtoPlayerState>())
	{
		const bool bNewReady = !PS->bIsReady;
		DEBUG_LOG("Toggling Ready → %s", bNewReady ? TEXT("true") : TEXT("false"));
		Server_SetReady(bNewReady);
	}
}


