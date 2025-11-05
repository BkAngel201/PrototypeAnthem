// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameModes/ProtoLobby_GM.h"

#include "Game/ProtoGameState.h"
#include "Player/ProtoPlayerState.h"
#include "Debug/ProtoDebug.h"

AProtoLobby_GM::AProtoLobby_GM()
{
}

void AProtoLobby_GM::CheckAllPlayersReady()
{
	bool bAllReady = true;
	int32 NumPlayers = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		DEBUG_LOG("[%s] Found PlayerState: %s", 
		HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), 
		*GetNameSafe(PS));
		if (AProtoPlayerState* ProtoPS = Cast<AProtoPlayerState>(PS))
		{
			NumPlayers++;
			if (!ProtoPS->bIsReady)
			{
				bAllReady = false;
			}
		}
	}

	if (bAllReady && NumPlayers > 0)
	{
		HandleAllPlayersReady();
	}
}

void AProtoLobby_GM::HandleAllPlayersReady()
{
	Super::HandleAllPlayersReady();
	DEBUG_LOG("All players ready — Starting countdown before travel.");
	
	if (AProtoGameState* GS = GetGameState<AProtoGameState>())
	{
		GS->CountdownEndTime = GetWorld()->GetTimeSeconds() + WaitTimeBeforeTravel;
		GS->OnRep_CountdownEndTime();
	}
	
	// Delay 5 seconds before actual map travel
	FTimerHandle TravelHandle;
	GetWorldTimerManager().SetTimer(TravelHandle, this, &AProtoLobby_GM::BeginTravelAfterCountdown, WaitTimeBeforeTravel, false);
}

void AProtoLobby_GM::BeginTravelAfterCountdown()
{
#if WITH_EDITOR
	// Force seamless travel in PIE by setting the console variable directly
	IConsoleVariable* SeamlessTravelVar = IConsoleManager::Get().FindConsoleVariable(TEXT("net.AllowPIESeamlessTravel"));
	if (SeamlessTravelVar)
	{
		SeamlessTravelVar->Set(1);
	}
#endif
	
	GetWorld()->ServerTravel("/Game/Maps/Mission_Test_Map", true);
	// use if the line above does not work.
	//UGameplayStatics::OpenLevel(this, FName("Mission_Test_Map"), true, "listen");
}
