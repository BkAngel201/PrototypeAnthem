// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/MissionManager/Objectives/Actors/ProtoMission_TargetLocation.h"

#include "Components/SphereComponent.h"
#include "Debug/ProtoDebug.h"
#include "GameFramework/Character.h"
#include "Managers/MissionManager/Objectives/Definitions/ProtoObjectiveDefinition_ReachLocation.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AProtoMission_TargetLocation::AProtoMission_TargetLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
	RootComponent = SphereTrigger;
	SphereTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereTrigger->SetSphereRadius(100.f);
	SphereTrigger->SetHiddenInGame(false);
	SphereTrigger->SetVisibility(true);
	SphereTrigger->bDrawOnlyIfSelected = false;
	SphereTrigger->SetIsReplicated(true);
}

void AProtoMission_TargetLocation::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &AProtoMission_TargetLocation::HandleOverlap);
	}
}

void AProtoMission_TargetLocation::InitFromDefinition(UProtoObjectiveDefinition_ReachLocation* Definition)
{
	SphereRadius = Definition->TargetLocationRadius;
	SphereTrigger->SetSphereRadius(SphereRadius);
}

void AProtoMission_TargetLocation::OnRep_SphereRadius()
{
	SphereTrigger->SetSphereRadius(SphereRadius);
}

void AProtoMission_TargetLocation::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProtoMission_TargetLocation, SphereRadius);
}

void AProtoMission_TargetLocation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		const FString RoleName = HasAuthority() ? TEXT("Server") : TEXT("Client");
		DEBUG_LOG("TargetLocation [%s]: Overlap detected by %s", *RoleName, *GetNameSafe(OtherActor));
		OnTargetReached.Broadcast(OtherActor);
	}
}


