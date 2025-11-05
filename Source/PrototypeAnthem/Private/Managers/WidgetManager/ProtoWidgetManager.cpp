// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/WidgetManager/ProtoWidgetManager.h"

#include "Blueprint/UserWidget.h"
#include "Game/ProtoGameState.h"
#include "Core/ProtoTypes.h"
#include "Managers/WidgetManager/ProtoScreenWidgetData.h"
#include "Debug/ProtoDebug.h"
#include "Engine/AssetManager.h"

void UProtoWidgetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UAssetManager::Get().GetPrimaryAssetDataList(UProtoTypeHelpers::GetAssetType_Widget(), ScreenWidgetsData);
}

void UProtoWidgetManager::Deinitialize()
{
	RemoveCurrentScreenWidget();
	CachedOwner = nullptr;
	ActiveScreenWidgets.Empty();
	ScreenWidgetsData.Empty();
	Super::Deinitialize();
}

void UProtoWidgetManager::SetCachedOwner(APlayerController* PC)
{
	if (PC != nullptr)
	{
		CachedOwner = PC;
		DEBUG_LOG("[WidgetManager] Cached Owner set to: %s", *GetNameSafe(CachedOwner));
	}
}

void UProtoWidgetManager::ShowScreenWidget()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	

	if (CachedOwner == nullptr)
	{
		DEBUG_LOG("⚠️ [WidgetManager] No PlayerController found yet!");
		return;
	}

	if (CachedOwner)
	{
		if (AProtoGameState* GS = GetWorld()->GetGameState<AProtoGameState>())
		{
			if (GS->GameContext != CurrentScreen)
			{
				RemoveCurrentScreenWidget();
				SpawnWidgetsForScreenWidgetData(GS->GameContext);
				DEBUG_LOG("[WidgetManager] Showing widget: %d | Owner: %s", 
				(uint8)GS->GameContext, *GetNameSafe(CachedOwner));
			}
		}
	}
}

void UProtoWidgetManager::RemoveCurrentScreenWidget()
{
	if (ActiveScreenWidgets.Num() != 0)
	{
		DEBUG_LOG("[WidgetManager] Removing Widget from Screen.");
		for (auto ActiveWidget : ActiveScreenWidgets)
		{
			ActiveWidget->RemoveFromParent();
		}
		ActiveScreenWidgets.Empty();
	}
}

void UProtoWidgetManager::SpawnWidgetsForScreenWidgetData(EGameContext GameContext)
{
	bool bWidgetsAdded = true;
	for (const FAssetData& ScreenData : ScreenWidgetsData)
	{
		UProtoScreenWidgetData* ScreenWidgetData = Cast<UProtoScreenWidgetData>(ScreenData.GetAsset());
		if (!ScreenWidgetData || ScreenWidgetData->WidgetToSpawnClass.Num() == 0) continue;

		if (ScreenWidgetData->UseWithGameContext == GameContext)
		{
			for (auto Widget : ScreenWidgetData->WidgetToSpawnClass)
			{
				if(IsValid(CachedOwner) && IsValid(CachedOwner->Player))
				{
					if (auto ActiveScreenWidget = CreateWidget<UUserWidget>(CachedOwner, Widget))
					{
						ActiveScreenWidgets.Add(ActiveScreenWidget);
						ActiveScreenWidget->AddToViewport();
						ActiveScreenWidget->SynchronizeProperties(); // ensures widget bindings are live
						ActiveScreenWidget->ForceLayoutPrepass();
					}
					else
					{
						bWidgetsAdded = false;	
					}
				}
				else
				{
					bWidgetsAdded = false;
				}
			}
		}
	}
	if (bWidgetsAdded)
	{
		CurrentScreen = GameContext;
	}
}
