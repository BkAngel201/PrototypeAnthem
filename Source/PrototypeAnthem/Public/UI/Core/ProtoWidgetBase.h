// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProtoWidgetBase.generated.h"

class UProtoEventHub;
class AProtoGameMode;
class AProtoGameState;
class AProtoPlayerState;
class AProtoPlayerController;
/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API UProtoWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void TryBindLater();
	
	UFUNCTION(BlueprintCallable, Category="System")
	AProtoPlayerController* GetPC() const { return PC; }
	UFUNCTION(BlueprintCallable, Category="System")
	AProtoPlayerState* GetPS();
	UFUNCTION(BlueprintCallable, Category="System")
	AProtoGameState* GetGS() const { return GS; }
	UFUNCTION(BlueprintCallable, Category="System")
	UProtoEventHub* GetEHub() const { return EHub; }
	UFUNCTION(BlueprintCallable, Category="System")
	float GetWorldTime() const;
	UFUNCTION(BlueprintPure)
	bool IsReady() const { return PC && GS && PS; }

	UFUNCTION(BlueprintImplementableEvent, Category="System")
	void SetupBindings();

	UFUNCTION(BlueprintImplementableEvent, Category="System")
	void CleanupBindings();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="System")
	AProtoPlayerController* PC;
	UPROPERTY(BlueprintReadOnly, Category="System")
	AProtoPlayerState* PS;
	UPROPERTY(BlueprintReadOnly, Category="System")
	AProtoGameState* GS;
	UPROPERTY(BlueprintReadOnly, Category="System")
	UProtoEventHub* EHub;
};
