// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ProtoGameMode.h"
#include "Debug/ProtoDebug.h"
#include "GameFramework/GameStateBase.h"

AProtoGameMode::AProtoGameMode()
{
	bUseSeamlessTravel = true;
}

void AProtoGameMode::BeginPlay()
{
	Super::BeginPlay();
	DEBUG_LOG("[GM Active] %s (Authority: %s)",
	   *GetClass()->GetName(),
	   HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"));
}

void AProtoGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	DEBUG_LOG("[GM_Base] Player joined.");
}

void AProtoGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	DEBUG_LOG("[GM_Base] Player left.");
}

void AProtoGameMode::HandleAllPlayersReady()
{
	if (!IsValid(GetWorld()) || GameState == nullptr) return;
	DEBUG_LOG("[GM_Base] All players ready (override in child).");
}


