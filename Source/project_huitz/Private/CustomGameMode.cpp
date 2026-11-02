// © 2026 Atlantic Games. All rights reserved.

#include "CustomGameMode.h"
#include "Player/CustomPlayerController.h"
#include "Multiplayer/MultiplayerGameState.h"
#include "Multiplayer/MultiplayerPlayerState.h"

ACustomGameMode::ACustomGameMode() {
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ACustomPlayerController::StaticClass();
	GameStateClass = AMultiplayerGameState::StaticClass();
	PlayerStateClass = AMultiplayerPlayerState::StaticClass();
}
