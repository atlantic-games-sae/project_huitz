// © 2026 Atlantic Games. All rights reserved.


#include "LevelScripts/GameplayLevelScript.h"
#include "CustomGameInstance.h"
#include "HelperFunctions.h"
#include "SaveSystem.h"
#include "Player/PlayerCharacter.h"

void AGameplayLevelScript::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(UHelperFunctions::GetCustomGameInstance(this)->GetMarksmanCharacter()) ||
		!IsValid(UHelperFunctions::GetCustomGameInstance(this)->GetBrawlerCharacter())) {
		UHelperFunctions::GetCustomGameInstance(this)->SpawnPlayerCharacters(0, 1);
	}
}

void AGameplayLevelScript::LoadFromLevelSaveData(FLevelSaveData& SaveData) {
	if (SaveData.Level.LoadSynchronous()->GetCurrentLevel() != GetLevel()) return;
	// do stuff with pickups data to apply the level state from the save data struct
}
