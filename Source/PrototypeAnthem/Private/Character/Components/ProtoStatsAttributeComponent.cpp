// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ProtoStatsAttributeComponent.h"

#include "Core/ProtoTypes.h"
#include "EventHub/ProtoEventHub.h"
#include "Net/UnrealNetwork.h"

UProtoStatsAttributeComponent::UProtoStatsAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	Health.Attribute = EAttribute::Health;
	Shield.Attribute = EAttribute::Shield;
}

void UProtoStatsAttributeComponent::OnRep_Health()
{
	DEBUG_LOG("AttributeComponent: OnRep_Health()");
	if (const auto Hub = GetEventHub())
	{
		DEBUG_LOG("AttributeComponent: OnRep_Health() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Health.Attribute, Health.CurrentValue, Health.GetDelta());
		Hub->BroadcastAttributeChanged(GetOwner(), Health.Attribute, Health.CurrentValue, Health.GetDelta());
	}
}

void UProtoStatsAttributeComponent::OnRep_Shield()
{
	DEBUG_LOG("AttributeComponent: OnRep_Shield()");
	if (const auto Hub = GetEventHub())
	{
		DEBUG_LOG("AttributeComponent: OnRep_Shield() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Shield.Attribute, Shield.CurrentValue, Shield.GetDelta());
		Hub->BroadcastAttributeChanged(GetOwner(), Shield.Attribute, Shield.CurrentValue, Shield.GetDelta());
	}
}

void UProtoStatsAttributeComponent::HandleDeath()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	if (!Owner || !Owner->HasAuthority()) return;
	
	DEBUG_LOG("AttributeComponent: HandleDeath()");
	OnDeath.Broadcast(Owner);
	// 🔹 Notify all clients to trigger their Blueprint OnDeath events
	Multicast_OnDeath(Owner);

	if (const auto Hub = GetEventHub())
	{
		DEBUG_LOG("AttributeComponent: HandleDeath() - %s Died.", *Owner->GetName());
		Hub->BroadcastCharacterDied(Owner);
	}
}

void UProtoStatsAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProtoStatsAttributeComponent, Health );
	DOREPLIFETIME(UProtoStatsAttributeComponent, Shield );
}

void UProtoStatsAttributeComponent::Multicast_OnDeath_Implementation(AActor* DeadActor)
{
	DEBUG_LOG("AttributeComponent: Multicast_OnDeath() - %s Died (Client Visuals).", *DeadActor->GetName());
	OnDeath.Broadcast(DeadActor);
}

void UProtoStatsAttributeComponent::ApplyDamage(float Amount, AActor* InstigatorActor)
{
	if (bIsDead) return;
	if (Amount <= 0) return;
	
	DEBUG_LOG("AttributeComponent: ApplyDamage() - Damage Amount: %f", Amount);
	float RemainingDamage = Amount;

	//Set previous Attivute Values
	Health.PreviousValue = Health.CurrentValue;
	Shield.PreviousValue = Shield.CurrentValue;
	
	if (Shield.CurrentValue > 0.f)
	{
		float ShieldAbsorption = FMath::Min(Shield.CurrentValue, RemainingDamage);
		Shield.CurrentValue -= ShieldAbsorption;
		RemainingDamage -= ShieldAbsorption;
		DEBUG_LOG("AttributeComponent: ApplyDamage() - Shield Absorbed Amount: %f", ShieldAbsorption);
	}

	if (RemainingDamage > 0.f)
	{
		Health.CurrentValue = FMath::Max(0.f, Health.CurrentValue - RemainingDamage);
		DEBUG_LOG("AttributeComponent: ApplyDamage() - Damage to Health: %f", RemainingDamage);
		if (Health.CurrentValue <= 0.f)
		{
			bIsDead = true;
			HandleDeath();
		}
	}
	
	if (const auto Hub = GetEventHub())
	{
		DEBUG_LOG("AttributeComponent: ApplyDamage() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Health.Attribute, Health.CurrentValue, Health.GetDelta());
		Hub->BroadcastAttributeChanged(GetOwner(), Health.Attribute, Health.CurrentValue, Health.GetDelta());
		DEBUG_LOG("AttributeComponent: ApplyDamage() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Shield.Attribute, Shield.CurrentValue, Shield.GetDelta());
		Hub->BroadcastAttributeChanged(GetOwner(), Shield.Attribute, Shield.CurrentValue, Shield.GetDelta());
	}
	
}

void UProtoStatsAttributeComponent::ApplyHeal(float Amount)
{
	DEBUG_LOG("AttributeComponent: ApplyHeal() - Heal Amount: %f", Amount);
	Health.CurrentValue = FMath::Min(Health.CurrentValue + Amount, Health.MaxValue);
	
	if (const auto Hub = GetEventHub())
	{
		DEBUG_LOG("AttributeComponent: ApplyHeal() - Attribute Changed: %d - NewValue: %f - Delta: %f", (uint8)Health.Attribute, Health.CurrentValue, Health.GetDelta());
		Hub->BroadcastAttributeChanged(GetOwner(), Health.Attribute, Health.CurrentValue, Health.GetDelta());
	}
}

void UProtoStatsAttributeComponent::Revive(float NewHealth)
{
}

UProtoEventHub* UProtoStatsAttributeComponent::GetEventHub()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UProtoEventHub>();
}




