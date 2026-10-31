// © 2026 Atlantic Games. All rights reserved.

#include "UI/ConfirmationDialogWidgetBase.h"

void UConfirmationDialogWidgetBase::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	OnConfirmationComplete.AddUniqueDynamic(this, &UConfirmationDialogWidgetBase::RemoveFromParentOnConfirmationComplete);
}

void UConfirmationDialogWidgetBase::RemoveFromParentOnConfirmationComplete(bool bResult) {
	RemoveFromParent();
}
