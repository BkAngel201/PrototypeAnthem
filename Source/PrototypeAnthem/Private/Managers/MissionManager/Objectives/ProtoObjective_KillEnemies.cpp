// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MissionManager/Objectives/ProtoObjective_KillEnemies.h"

#include "EventHub/ProtoEventHub.h"
#include "Debug/ProtoDebug.h"
#include "Managers/MissionManager/ProtoMissionManager.h"
#include "Managers/MissionManager/Objectives/Actors/ProtoMission_EnemySpawn.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition_KillEnemies.h"

UProtoObjective_KillEnemies::UProtoObjective_KillEnemies()
{
	CurrentKills = 0;
	TargetActorClass = AProtoMission_EnemySpawn::StaticClass();
}

void UProtoObjective_KillEnemies::InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def,
	UProtoMissionManager* Owner_MM)
{
	Super::InitializeFromDefinition(Index, Def, Owner_MM);
	ObjectiveDefinition = Cast<UProtoObjectiveDefinition_KillEnemies>(Def);
	DEBUG_LOG("⚠️ InitializeFromDefinition(KillEnemies): %s", *GetName());
}

void UProtoObjective_KillEnemies::HandleOnCharacterDie(AActor* DeadActor)
{
	if (!HasAuthority() || bInitialized == false) return;
	if (ObjectiveState != EObjectiveState::Active) return;
	if (!ObjectiveDefinition) return;

	if (ObjectiveDefinition->EnemiesRequirements.Num() > 0)
	{
		bool bMatch = false;
		for (auto Enemy : ObjectiveDefinition->EnemiesRequirements)
		{
			if (DeadActor->IsA(Enemy.TargetEnemyClass))
			{
				bMatch = true;
				break;
			}
		}
		if (!bMatch) return;
	}

	CurrentKills++;
	SetObjectiveCustomData();
	DEBUG_LOG("Objective_KillEnemies: Enemy killed %d / %d", CurrentKills, RequiredKills);
	

	if (CurrentKills >= RequiredKills)
	{
		if (UProtoEventHub* Hub = GetEventHub())
		{
			Hub->OnCharacterDied.RemoveDynamic(this, &UProtoObjective_KillEnemies::HandleOnCharacterDie);
		}
		CompleteObjective();
	}
	
}

void UProtoObjective_KillEnemies::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	if (!HasAuthority()) return;
	
	if (ObjectiveDefinition == nullptr) return;

	if (Owner_MissionManager && Owner_MissionManager->GetWorld() && Owner_MissionManager->GetWorld()->GetAuthGameMode())
	{
		TargetActor = Owner_MissionManager->GetWorld()->SpawnActor<AProtoMission_EnemySpawn>(TargetActorClass, ObjectiveDefinition->TargetTransform);
		if (TargetActor)
		{
			TargetActor->InitFromDefinition(ObjectiveDefinition);
		}
	}
	else
	{
		DEBUG_LOG("❌ No valid world or GameMode in Objective_ReachLocation for %s", *GetName());
	}
	
	RequiredKills = 0;
	
	for (auto Enemy : ObjectiveDefinition->EnemiesRequirements)
	{
		RequiredKills += Enemy.RequiredKills;
	}

	SetObjectiveCustomData();
	DEBUG_LOG("Objective_KillEnemies: RequiredKills: %d", RequiredKills);
	if (UProtoEventHub* Hub = GetEventHub())
	{
		Hub->OnCharacterDied.AddDynamic(this, &UProtoObjective_KillEnemies::HandleOnCharacterDie);
		DEBUG_LOG("Objective_KillEnemies: Subscribed to OnCharacterDied");
	}
	
	
}

void UProtoObjective_KillEnemies::FailObjective_Implementation()
{
	Super::FailObjective_Implementation();
	if (UProtoEventHub* Hub = GetEventHub())
	{
		Hub->OnCharacterDied.RemoveDynamic(this, &UProtoObjective_KillEnemies::HandleOnCharacterDie);
	}
}

void UProtoObjective_KillEnemies::SetObjectiveCustomData()
{
	TArray<FObjectiveDataPair> DataUpdated;
	DataUpdated.Add({ FGameplayTag::RequestGameplayTag("MissionManager.Objective.KillEnemies.CurrentKills"), static_cast<float>(CurrentKills), TEXT("Enemies Killed") });
	DataUpdated.Add({ FGameplayTag::RequestGameplayTag("MissionManager.Objective.KillEnemies.RequiredKills"), static_cast<float>(RequiredKills), TEXT("Kills Required") });
	SetObjectiveDataUpdated(DataUpdated);
}
