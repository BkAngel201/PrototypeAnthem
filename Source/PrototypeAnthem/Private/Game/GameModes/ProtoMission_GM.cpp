// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameModes/ProtoMission_GM.h"

#include "Game/ProtoGameState.h"
#include "Debug/ProtoDebug.h"

void AProtoMission_GM::BeginPlay()
{
	Super::BeginPlay();
}

void AProtoMission_GM::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	DEBUG_LOG("Mission GameMode: PostSeamlessTravel() — starting HUD + countdown setup");
	
	if (AProtoGameState* GS = GetGameState<AProtoGameState>())
	{
		GS->CountdownEndTime = GetWorld()->GetTimeSeconds() + WaitTimeBeforeStart;
		GS->OnRep_CountdownEndTime();
	}
			
	DEBUG_LOG("Mission GameMode: PostSeamlessTravel finished — HUDs created, countdown started");
}

void AProtoMission_GM::TravelToLobby()
{
	GetWorld()->ServerTravel("/Game/Maps/Lobby_Map", true);
}
