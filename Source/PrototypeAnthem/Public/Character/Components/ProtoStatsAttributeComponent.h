// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProtoStatsAttributeComponent.generated.h"

enum class EAttribute : uint8;
class UProtoEventHub;

USTRUCT(BlueprintType)
struct FProtoAttribute
{
	GENERATED_BODY();

	UPROPERTY()
	EAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PreviousValue = CurrentValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxValue = 100.f;

	float GetPercent() const { return MaxValue > 0 ? CurrentValue / MaxValue : 0.f; }
	float GetDelta() const { return CurrentValue - PreviousValue; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPEANTHEM_API UProtoStatsAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProtoStatsAttributeComponent();

protected:

	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Attributes|Health")
	FProtoAttribute Health;
	UPROPERTY(ReplicatedUsing=OnRep_Shield, VisibleAnywhere, BlueprintReadOnly, Category="Attributes|Shield")
	FProtoAttribute Shield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes|Health")
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Shield();

	UFUNCTION()
	void HandleDeath();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	UPROPERTY(BlueprintAssignable, Category="Attributes|Events")
	FOnDeath OnDeath;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath(AActor* DeadActor);

	UFUNCTION(BlueprintCallable, Category="Attributes|Health")
	void ApplyDamage(float Amount, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attributes|Health")
	void ApplyHeal(float Amount);
	
	UFUNCTION(BlueprintPure, Category="Attributes|Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category="Attributes|Health")
	void Revive(float NewHealth = 50.f);


	// Attribute Getters
	UFUNCTION(BlueprintPure, Category="Attributes|Health")
	float GetHealth() const { return Health.CurrentValue; }

	UFUNCTION(BlueprintPure, Category="Attributes|Health")
	float GetMaxHealth() const { return Health.MaxValue; }

	UFUNCTION(BlueprintPure, Category="Attributes|Health")
	float GetHealthPercent() const { return Health.GetPercent(); }

	UFUNCTION(BlueprintPure, Category="Attributes|Shield")
	float GetShield() const { return Shield.CurrentValue; }

	UFUNCTION(BlueprintPure, Category="Attributes|Shield")
	float GetMaxShield() const { return Shield.MaxValue; }

	UFUNCTION(BlueprintPure, Category="Attributes|Shield")
	float GetShieldPercent() const { return Shield.GetPercent(); }

	UFUNCTION()
	UProtoEventHub* GetEventHub();
};
