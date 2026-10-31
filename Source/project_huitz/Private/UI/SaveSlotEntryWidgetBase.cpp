// © 2026 Atlantic Games. All rights reserved.

#include "UI/SaveSlotEntryWidgetBase.h"

#include "HelperFunctions.h"
#include "Components/ListViewBase.h"
#include "LevelScripts/MainMenuLevelScript.h"
#include "UI/SaveSelectionWidgetBase.h"

void USaveSlotEntryWidgetBase::ClearSaveSlot(int SaveSlotIndex) {
	TargetSlotToClear = SaveSlotIndex;
	UConfirmationDialogWidgetBase* ConfirmationDialog = CreateConfirmationDialog();
	ConfirmationDialog->SetConfirmationMessage(FString(TEXT("Do you really want to clear all data from this save slot? This action cannot be reversed.")));
	ConfirmationDialog->OnConfirmationComplete.AddUniqueDynamic(this, &USaveSlotEntryWidgetBase::OnClearSaveSlotConfirmation);
}

void USaveSlotEntryWidgetBase::OnClearSaveSlotConfirmation(bool bResult) {
	if (bResult) {
		UHelperFunctions::GetCustomGameInstance(this)->ClearSaveSlot(TargetSlotToClear);
		Cast<USaveSelectionWidgetBase>(GetBackend()->ForegroundWidget_Ref)->Construct();
	}
}
