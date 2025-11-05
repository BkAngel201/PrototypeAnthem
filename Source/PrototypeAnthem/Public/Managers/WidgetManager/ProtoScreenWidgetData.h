#pragma once
#include "Blueprint/UserWidget.h"
#include "ProtoScreenWidgetData.generated.h"

enum class EGameContext : uint8;

UCLASS(BlueprintType)
class  UProtoScreenWidgetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	EGameContext UseWithGameContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	TArray<TSubclassOf<UUserWidget>> WidgetToSpawnClass;

	// Optional — for debugging or future grouping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget")
	FText DisplayName;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("WidgetDefinition"), GetFName());
	}
};
