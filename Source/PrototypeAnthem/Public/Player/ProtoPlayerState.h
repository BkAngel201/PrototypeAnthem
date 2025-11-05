// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProtoPlayerState.generated.h"

/**
 * 
 */
enum class ERoleType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyDataChanged, AProtoPlayerState*, PlayerState);

UCLASS()
class PROTOTYPEANTHEM_API AProtoPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AProtoPlayerState();
	virtual void BeginPlay() override;
	void OnRep_PlayerName() override;
	
	// --- Replicated Lobby Data ---
	UPROPERTY(ReplicatedUsing=OnRep_SelectedRole, BlueprintReadOnly, Category="Lobby")
	ERoleType SelectedRole;

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
