// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Managers/MissionManager/Missions/ProtoMissionDefinition.h"
#include "ProtoGameState.generated.h"

class UProtoEventHub;
/**
 * 
 */
enum class EMissionState: uint8;
class UProtoMissionManager;
struct FMissionRuntimeData;

UENUM(BlueprintType)
enum class EGameContext : uint8
{
	None,
	Lobby,
	Mission,
	Other
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerArrayChanged, AProtoGameState*, GameStateRef);

UCLASS()
class PROTOTYPEANTHEM_API AProtoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AProtoGameState();
	virtual void BeginPlay() override;
	void SetupMissionManager();
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UPROPERTY(BlueprintAssignable, Category="Events | Lobby")
	FOnPlayerArrayChanged OnPlayerArrayChanged;

	UPROPERTY(ReplicatedUsing=OnRep_CountdownEndTime, BlueprintReadOnly, Category="Mission")
	float CountdownEndTime;

	UPROPERTY(ReplicatedUsing=OnRep_MissionData, BlueprintReadOnly, Category="Mission")
	FMissionRuntimeData CurrentMissionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Managers")
	TSubclassOf<UProtoMissionManager>  MissionManagerClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers")
	UProtoMissionManager*  MissionManager;
	
	UFUNCTION(BlueprintPure, Category="Mission")
	UProtoMissionManager* GetMissionManager() const { return MissionManager; }

	UFUNCTION()
	UProtoEventHub* GetEventHub() const;

	UPROPERTY(ReplicatedUsing = OnRep_GameContext, BlueprintReadOnly, Category="Context")
	EGameContext GameContext;

	// --- Networking ---
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_CountdownEndTime();
	UFUNCTION()
	void OnRep_MissionData();
	UFUNCTION()
	void OnRep_GameContext();
	
	UFUNCTION()
	void UpdateMissionDataSnapshot(const FMissionRuntimeData& MissionData);
};
