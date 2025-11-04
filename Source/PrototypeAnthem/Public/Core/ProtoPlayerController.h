// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoPlayerController.generated.h"

enum class ERole_Type : uint8;
/**
 * 
 */
UCLASS()
class PROTOTYPEANTHEM_API AProtoPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AProtoPlayerController();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SelectRole(ERole_Type InRole);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetReady(bool bInReady);

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void SelectRole(ERole_Type NewRole);

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void SetReady(bool bNewReady);

	UFUNCTION(BlueprintCallable, Category="Lobby")
	void ToggleReady();
};