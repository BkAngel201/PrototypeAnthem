// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoPlayerController.generated.h"

class UProtoScreenWidgetData;
enum class ERoleType : uint8;
class UProtoWidgetManager;
class UUserWidget;
/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API AProtoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AProtoPlayerController();
	virtual void BeginPlay() override;
	
	void DeferredShowWidgets();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UProtoScreenWidgetData* WidgetData;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SelectRole(ERoleType InRole);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetReady(bool bInReady);
	

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void SelectRole(ERoleType NewRole);

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void SetReady(bool bNewReady);

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void ToggleReady();

	FTimerHandle ContextCheckHandle;

};