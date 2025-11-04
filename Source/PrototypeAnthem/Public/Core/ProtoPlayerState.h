// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProtoPlayerState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ERole_Type : uint8
{
	Tank = 0,
	DPS,
	Utility,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyDataChanged, AProtoPlayerState*, PlayerState);

UCLASS()
class PROTOTYPEANTHEM_API AProtoPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AProtoPlayerState();
	
	// --- Replicated Lobby Data ---
	UPROPERTY(ReplicatedUsing=OnRep_SelectedRole, BlueprintReadOnly, Category="Lobby")
	ERole_Type SelectedRole;

	UPROPERTY(ReplicatedUsing=OnRep_IsReady, BlueprintReadOnly, Category="Lobby")
	bool bIsReady;

	// --- Delegates for UI / GameState ---
	UPROPERTY(BlueprintAssignable, Category="Lobby")
	FOnLobbyDataChanged OnLobbyDataChanged;
	
	// --- RepNotifies ---
	UFUNCTION()
	void OnRep_SelectedRole();
	
	UFUNCTION()
	void OnRep_IsReady();

	// --- Networking ---
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
