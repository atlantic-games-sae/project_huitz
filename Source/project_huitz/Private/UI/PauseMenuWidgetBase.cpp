// © 2026 Atlantic Games. All rights reserved.

#include "UI/PauseMenuWidgetBase.h"

#include "Player/Components/PauseMenuComponent.h"
#include "UI/ConfirmationDialogWidgetBase.h"

UPauseMenuComponent* UPauseMenuWidgetBase::GetBackend() const {
	return GetOwningPlayer()->GetComponentByClass<UPauseMenuComponent>();
}

TObjectPtr<UConfirmationDialogWidgetBase> UPauseMenuWidgetBase::CreateConfirmationDialog() const {
	UConfirmationDialogWidgetBase* ConfirmationDialog = Super::CreateConfirmationDialog();
	ConfirmationDialog->AddToPlayerScreen(2);
	return ConfirmationDialog;
}
