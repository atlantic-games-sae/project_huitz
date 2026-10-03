// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerGameMode.h"
#include "PlayerCharacter.h"
#include "CustomPlayerController.h"
#include "MultiplayerGameState.h"
#include "MultiplayerPlayerState.h"
#include "Kismet/GameplayStatics.h"

AMultiplayerGameMode::AMultiplayerGameMode() {
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ACustomPlayerController::StaticClass();
	GameStateClass = AMultiplayerGameState::StaticClass();
	PlayerStateClass = AMultiplayerPlayerState::StaticClass();
}

void AMultiplayerGameMode::BeginPlay() {
	Super::BeginPlay();
}
