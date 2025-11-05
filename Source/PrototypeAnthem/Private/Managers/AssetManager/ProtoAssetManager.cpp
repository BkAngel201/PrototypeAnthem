// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AssetManager/ProtoAssetManager.h"

#include "Managers/MissionManager/Missions/ProtoMissionDefinition.h"

UProtoAssetManager& UProtoAssetManager::Get()
{
	return *Cast<UProtoAssetManager>(GEngine->AssetManager);
}

void UProtoAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UE_LOG(LogTemp, Log, TEXT("[AssetManager] Initialized and scanning assets..."));
}

UProtoMissionDefinition* UProtoAssetManager::LoadMissionById(FPrimaryAssetId AssetId) const
{
	FAssetData AssetData;
	GetPrimaryAssetData(AssetId, AssetData);

	if (AssetData.IsValid())
	{
		return Cast<UProtoMissionDefinition>(AssetData.GetAsset());
	}

	return nullptr;
}
