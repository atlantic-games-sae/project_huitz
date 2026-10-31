// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuWidgetBase.h"
#include "SaveSelectionWidgetBase.generated.h"

class USaveSlot_SaveGame;

UCLASS(BlueprintType)
class PROJECT_HUITZ_API USaveSlotDisplayInfo : public UObject {
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsEmptySlot;
	
	UPROPERTY(BlueprintReadOnly)
	int SlotIndex;
	
	UPROPERTY(BlueprintReadOnly)
	float TotalPlayTimeHours;
};

UCLASS(Abstract)
class PROJECT_HUITZ_API USaveSelectionWidgetBase : public UMainMenuWidgetBase {
	GENERATED_BODY()
	
protected:
	USaveSlotDisplayInfo* ConvertSaveGameToDisplayInfo(int SlotIndex, const USaveSlot_SaveGame* SaveGame);
	
	USaveSlotDisplayInfo* GetEmptySlotDisplayInfo(int SlotIndex);
	
	UFUNCTION(BlueprintPure, Category="Save Slots")
	TArray<USaveSlotDisplayInfo*> GetAllSavesAsDisplayInfo();
};
