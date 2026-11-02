// © 2026 Atlantic Games. All rights reserved.

#include "Multiplayer/MultiplayerGameMode.h"

#include "HelperFunctions.h"
#include "Kismet/GameplayStatics.h"

void AMultiplayerGameMode::BeginPlay() {
	Super::BeginPlay();
	
	// if there is only one player controller, then uses SpawnPlayerCharacters to spawn and possess the two characters
	// in a build this should never run, since the second player controller is already created in the main menu
	if (!IsValid(UGameplayStatics::GetPlayerController(this, 1))) {
		UHelperFunctions::GetCustomGameInstance(this)->SpawnPlayerCharacters(0, 1);
	}
}
