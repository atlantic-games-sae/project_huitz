// © 2026 Atlantic Games. All rights reserved.

#include "UI/SaveSelectionWidgetBase.h"

#include "HelperFunctions.h"
#include "SaveSystem.h"

USaveSlotDisplayInfo* USaveSelectionWidgetBase::ConvertSaveGameToDisplayInfo(int SlotIndex, const USaveSlot_SaveGame* SaveGame) {
	USaveSlotDisplayInfo* ReturnValue = NewObject<USaveSlotDisplayInfo>();
	
	if (!IsValid(SaveGame)) {
		ReturnValue->bIsEmptySlot = true;
	} else {
		ReturnValue->SlotIndex = SlotIndex;
		ReturnValue->TotalPlayTimeHours = FMath::RoundHalfFromZero(SaveGame->TotalPlayTime / 36.0f) / 10.0f;
	}
	
	return ReturnValue;
}

USaveSlotDisplayInfo* USaveSelectionWidgetBase::GetEmptySlotDisplayInfo(int SlotIndex) {
	USaveSlotDisplayInfo* ReturnValue = NewObject<USaveSlotDisplayInfo>();
	
	ReturnValue->bIsEmptySlot = true;
	ReturnValue->SlotIndex = SlotIndex;
	
	return ReturnValue;
}

TArray<USaveSlotDisplayInfo*> USaveSelectionWidgetBase::GetAllSavesAsDisplayInfo() {
	TArray<USaveSlotDisplayInfo*> ReturnValue;
	for (int i = 0; i < UHelperFunctions::GetCustomGameInstance(this)->SaveSlotNames.Num(); i++) {
		FString SlotName = UHelperFunctions::GetCustomGameInstance(this)->SaveSlotNames[i];
		USaveSlot_SaveGame* SaveGame = Cast<USaveSlot_SaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (IsValid(SaveGame)) {
			USaveSlotDisplayInfo* SaveSlotDisplayInfo = ConvertSaveGameToDisplayInfo(i, SaveGame);
			ReturnValue.AddUnique(SaveSlotDisplayInfo);
		} else {
			ReturnValue.AddUnique(GetEmptySlotDisplayInfo(i));
		}
	}
	return ReturnValue;
}
