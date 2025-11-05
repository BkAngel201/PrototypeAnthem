// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ProtoGameMode.h"
#include "ProtoMission_GM.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API AProtoMission_GM : public AProtoGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "Mission", EditAnywhere)
	float WaitTimeBeforeStart = 1.0f;

	UFUNCTION()
	virtual void BeginPlay() override;
	virtual void PostSeamlessTravel() override;
	

	void TravelToLobby();
	
};
