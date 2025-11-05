// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/ProtoWidgetBase.h"

#include "Player/ProtoPlayerController.h"
#include "Player/ProtoPlayerState.h"
#include "Game/ProtoGameState.h"
#include "EventHub/ProtoEventHub.h"
#include "Debug/ProtoDebug.h"

void UProtoWidgetBase::NativeConstruct()
{
	PC = Cast<AProtoPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PS = PC->GetPlayerState<AProtoPlayerState>();
	}
	
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			EHub = GI->GetSubsystem<UProtoEventHub>();
		}
	}
	
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UProtoWidgetBase::TryBindLater);
}

void UProtoWidgetBase::TryBindLater()
{
	GS = GetWorld()->GetGameState<AProtoGameState>();

	if (IsValid(GS))
	{
		DEBUG_LOG("[%s] Widget successfully bound.", *GetName());
		SetupBindings();
	}
	else
	{
		static int32 RetryCount = 0;
		if (RetryCount++ < 20) // try for ~5 seconds max (20 × 0.25s)
		{
			DEBUG_LOG("[%s] Widget not ready yet (%d), retrying...", *GetName(), RetryCount);
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UProtoWidgetBase::TryBindLater);
		}
		else
		{
			DEBUG_LOG("[%s] Failed to find GameState after retries.", *GetName());
		}
	}
}

AProtoPlayerState* UProtoWidgetBase::GetPS()
{
	if (!IsValid(PS) && IsValid(PC))
	{
		PS = PC->GetPlayerState<AProtoPlayerState>();
	}
	return PS;
}

void UProtoWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
	CleanupBindings();
}

float UProtoWidgetBase::GetWorldTime() const
{
	return GetWorld()->GetTimeSeconds();
}
