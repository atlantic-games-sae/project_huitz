// © 2026 Atlantic Games. All rights reserved.

#include "UI/MainMenuWidgetBase.h"
#include "LevelScripts/MainMenuLevelScript.h"
#include "CustomGameInstance.h"
#include "HelperFunctions.h"

AMainMenuLevelScript* UMainMenuWidgetBase::GetBackend() const {
	return static_cast<AMainMenuLevelScript*>(GetWorld()->GetLevelScriptActor());
}

TObjectPtr<UConfirmationDialogWidgetBase> UMainMenuWidgetBase::CreateConfirmationDialog() const {
	UConfirmationDialogWidgetBase* ConfirmationDialog = Super::CreateConfirmationDialog();
	ConfirmationDialog->AddToViewport(2);
	return ConfirmationDialog;
}
