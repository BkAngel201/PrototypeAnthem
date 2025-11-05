
#include "Managers/MissionManager/Objectives/Actors/ProtoMission_EnemySpawn.h"

#include "Debug/ProtoDebug.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition_KillEnemies.h"

AProtoMission_EnemySpawn::AProtoMission_EnemySpawn()
{
	
}

void AProtoMission_EnemySpawn::BeginPlay()
{
	Super::BeginPlay();
}

void AProtoMission_EnemySpawn::InitFromDefinition(UProtoObjectiveDefinition_KillEnemies* Definition)
{
	if (Definition->EnemiesRequirements.Num() == 0) return;
	
	for (auto Enemy : Definition->EnemiesRequirements)
	{
		UWorld* World = GetWorld();
		if (!World || !Enemy.TargetEnemyClass)
		{
			return;
		}

		for (int i = 0; i < Enemy.RequiredKills; ++i)
		{
			FVector2D Random2D = FMath::RandPointInCircle( Definition->SpawnRadius);
			FVector RandomOffset(Random2D.X, Random2D.Y, 0.0f);

			FVector SpawnLocation = Definition->TargetTransform.GetLocation() + RandomOffset;
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			// SpawnParams.Owner = this; // If this actor should own the spawned actor

			// Spawn the actor
			AActor* SpawnedActor = World->SpawnActor<AActor>(Enemy.TargetEnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnedActor)
			{
				const FString RoleName = HasAuthority() ? TEXT("Server") : TEXT("Client");
				DEBUG_LOG("EnemySpawn [%s]: Overlap detected by %s", *RoleName, *GetName());
			}
		}
		
	}
}
