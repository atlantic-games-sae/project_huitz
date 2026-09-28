// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_HUITZ_API AMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Multiplayer")
	int32 LocalPlayerIndex = 0;
};
