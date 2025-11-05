// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ProtoAssetManager.generated.h"

class UProtoMissionDefinition;
/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API UProtoAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UProtoAssetManager& Get();

	virtual void StartInitialLoading() override;

	UFUNCTION(BlueprintCallable, Category="Assets")
	UProtoMissionDefinition* LoadMissionById(FPrimaryAssetId AssetId) const;
	
};
