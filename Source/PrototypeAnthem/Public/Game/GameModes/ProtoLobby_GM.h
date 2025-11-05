// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ProtoGameMode.h"
#include "ProtoLobby_GM.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API AProtoLobby_GM : public AProtoGameMode
{
	GENERATED_BODY()
public:
	AProtoLobby_GM();
	
	UPROPERTY(Category = "Lobby", EditAnywhere)
	float WaitTimeBeforeTravel = 1.0f;
	
	void CheckAllPlayersReady();


protected:
	virtual void HandleAllPlayersReady() override;
	
	void BeginTravelAfterCountdown();
};
