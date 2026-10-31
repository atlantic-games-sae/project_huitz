// © 2026 Atlantic Games. All rights reserved.


#include "LevelScripts/GameplayLevelScript.h"
#include "CustomGameInstance.h"
#include "SaveSystem.h"

void AGameplayLevelScript::LoadFromLevelSaveData(FLevelSaveData& SaveData) {
	if (SaveData.Level.LoadSynchronous()->GetCurrentLevel() != GetLevel()) return;
	// do stuff with pickups data to apply the level state from the save data struct
}
