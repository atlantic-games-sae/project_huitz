// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

UCLASS()
class PROJECT_HUITZ_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMultiplayerGameMode();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> BackgroundWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> LobbySelectWidgetClass;
	
protected:
	virtual void BeginPlay() override;
};
