// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SaveSlotEntryWidgetBase.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API USaveSlotEntryWidgetBase : public UMainMenuWidgetBase, public IUserObjectListEntry {
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category="Save Slots")
	void ClearSaveSlot(int SaveSlotIndex);
	
private:
	UFUNCTION()
	void OnClearSaveSlotConfirmation(bool bResult);
	
	int TargetSlotToClear;
};
