// © 2026 Atlantic Games. All rights reserved.


#include "LevelScripts/GameplayLevelScript.h"
#include "CustomGameInstance.h"
#include "Libraries/HelperFunctions.h"
#include "SaveSystem.h"
#include "Player/PlayerCharacter.h"

void AGameplayLevelScript::BeginPlay() {
	Super::BeginPlay();
	
	UHelperFunctions::GetCustomGameInstance(this)->OnGameplayLevelBeginPlay();
}

void AGameplayLevelScript::LoadFromLevelSaveData(FLevelSaveData& SaveData) {
	if (UHelperFunctions::GetCustomGameInstance(this)->GameLevels[SaveData.LevelIndex]->GetCurrentLevel() != GetLevel()) return;
	// do stuff with pickups data to apply the level state from the save data struct
}
