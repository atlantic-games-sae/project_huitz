// © 2026 Atlantic Games. All rights reserved.

#include "UI/WidgetBase.h"

#include "HelperFunctions.h"
#include "UI/ConfirmationDialogWidgetBase.h"

TObjectPtr<UConfirmationDialogWidgetBase> UWidgetBase::CreateConfirmationDialog() const {
	UConfirmationDialogWidgetBase* ConfirmationDialog = CreateWidget<UConfirmationDialogWidgetBase>(GetOwningPlayer(), UHelperFunctions::GetCustomGameInstance(this)->ConfirmationDialogWidgetClass);
	return ConfirmationDialog;
}
