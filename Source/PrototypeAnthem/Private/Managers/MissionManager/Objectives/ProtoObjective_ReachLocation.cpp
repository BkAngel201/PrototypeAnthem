// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/MissionManager/Objectives/ProtoObjective_ReachLocation.h"

#include "Components/SphereComponent.h"
#include "Debug/ProtoDebug.h"
#include "Managers/MissionManager/ProtoMissionManager.h"
#include "Managers/MissionManager/Objectives/Actors/ProtoMission_TargetLocation.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition_ReachLocation.h"

UProtoObjective_ReachLocation::UProtoObjective_ReachLocation()
{
	TargetActorClass = AProtoMission_TargetLocation::StaticClass();
}

void UProtoObjective_ReachLocation::InitializeFromDefinition(int32 Index, UProtoObjectiveDefinition* Def,
	UProtoMissionManager* Owner_MM)
{
	Super::InitializeFromDefinition(Index, Def, Owner_MM);
	ObjectiveDefinition = Cast<UProtoObjectiveDefinition_ReachLocation>(Def);
	DEBUG_LOG("⚠️ InitializeFromDefinition(ReachLocation): %s", *GetName());
}

void UProtoObjective_ReachLocation::StartObjective_Implementation()
{
	Super::StartObjective_Implementation();
	
	if (ObjectiveDefinition->ObjectiveClass == nullptr) return;
	
	if (Owner_MissionManager && Owner_MissionManager->GetWorld() && Owner_MissionManager->GetWorld()->GetAuthGameMode())
	{
		TargetActor = Owner_MissionManager->GetWorld()->SpawnActor<AProtoMission_TargetLocation>(TargetActorClass, ObjectiveDefinition->TargetTransform);
		if (TargetActor)
		{
			TargetActor->InitFromDefinition(ObjectiveDefinition);
			DEBUG_LOG("Objective_ReachLocation: Spawning Actor() %s", *GetNameSafe(TargetActor));
			TargetActor->OnTargetReached.AddDynamic(this, &UProtoObjective_ReachLocation::OnTargetReached);
			DEBUG_LOG("Objective_ReachLocation: Binding Finished");
		}
	}
	else
	{
		DEBUG_LOG("❌ No valid world or GameMode in Objective_ReachLocation for %s", *GetName());
	}
	
}

void UProtoObjective_ReachLocation::OnTargetReached(AActor* Player)
{
	const FString RoleName = HasAuthority() ? TEXT("Server") : TEXT("Client");
	DEBUG_LOG("Objective_ReachLocation [%s]: Target reached by %s", *RoleName, *GetNameSafe(Player));
	CompleteObjective();
	if (TargetActor && !TargetActor->IsPendingKillPending())
	{
		TargetActor->OnTargetReached.RemoveAll(this);
		TargetActor->Destroy();
		TargetActor = nullptr;
	}
}
