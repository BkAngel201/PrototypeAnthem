// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProtoMission_TargetLocation.generated.h"

class UProtoObjectiveDefinition_ReachLocation;
class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetReached, AActor*, ReachedActor);
UCLASS()
class PROTOTYPEANTHEM_API AProtoMission_TargetLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	AProtoMission_TargetLocation();

protected:
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitFromDefinition(UProtoObjectiveDefinition_ReachLocation* Definition);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereTrigger;

	UPROPERTY(ReplicatedUsing=OnRep_SphereRadius)
	float SphereRadius;
	
	UFUNCTION()
	void OnRep_SphereRadius();
	//
	UPROPERTY(BlueprintAssignable)
	FOnTargetReached OnTargetReached;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
