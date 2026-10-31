// © 2026 Atlantic Games. All rights reserved.

#include "CustomGameInstance.h"
#include "SaveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "LevelScripts/GameplayLevelScript.h"

void UCustomGameInstance::SaveGameToActiveSlot() {
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
	}
	ActiveSaveSlot_SaveGame->LevelSaveData = {
		.Level = GetWorld()
		// pickup related stuff here
	};
	
	ActiveSaveSlot_SaveGame->TotalPlayTime += UGameplayStatics::GetTimeSeconds(this) - TimestampOfLastSave;
	UpdateTimestampOfLastSave();
	
	// Brawler and marksman save data should be gathered and added here
	
	UGameplayStatics::SaveGameToSlot(ActiveSaveSlot_SaveGame, SaveSlotNames[ActiveSaveSlotIndex], 0);
}

void UCustomGameInstance::LoadGameFromActiveSlot() {
	ActiveSaveSlot_SaveGame = Cast<USaveSlot_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotNames[ActiveSaveSlotIndex], 0));
	UpdateTimestampOfLastSave();
	if (!IsValid(ActiveSaveSlot_SaveGame)) {
		ActiveSaveSlot_SaveGame = NewObject<USaveSlot_SaveGame>();
		ActiveSaveSlot_SaveGame->LevelSaveData.Level = GameLevels[0];
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ActiveSaveSlot_SaveGame->LevelSaveData.Level);
		SaveGameToActiveSlot();
	} else {
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, ActiveSaveSlot_SaveGame->LevelSaveData.Level);
	}
	AGameplayLevelScript* GameplayLevelScript = Cast<AGameplayLevelScript>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	if (!IsValid(GameplayLevelScript)) return;
	GameplayLevelScript->LoadFromLevelSaveData(ActiveSaveSlot_SaveGame->LevelSaveData);
}

void UCustomGameInstance::ClearSaveSlot(int SaveSlotIndex) {
	UGameplayStatics::DeleteGameInSlot(SaveSlotNames[SaveSlotIndex], 0);
}

void UCustomGameInstance::UpdateTimestampOfLastSave() {
	TimestampOfLastSave = UGameplayStatics::GetTimeSeconds(this);
}
