// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

class UConfirmationDialogWidgetBase;

UCLASS(Abstract)
class PROJECT_HUITZ_API UWidgetBase : public UUserWidget {
	GENERATED_BODY()
	
protected:
	virtual TObjectPtr<UConfirmationDialogWidgetBase> CreateConfirmationDialog() const;
};
