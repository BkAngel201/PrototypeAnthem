// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProtoGameMode.h"

#include "Core/ProtoPlayerState.h"
#include "Debug/ProtoDebug.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

AProtoGameMode::AProtoGameMode()
{
}

void AProtoGameMode::CheckAllPlayersReady()
{
	bool bAllReady = true;
	int32 NumPlayers = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
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
		UE_LOG(LogProto, Warning, TEXT("All players ready — traveling to Mission Map"));
		GetWorld()->ServerTravel("/Game/Maps/Mission_Test_Map?listen");
		// use if the line above does not work
		//UGameplayStatics::OpenLevel(this, FName("Mission_Test_Map"), true, "listen");
	}
}
