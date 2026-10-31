// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidgetBase.generated.h"

class UPauseMenuComponent;

UCLASS(Abstract)
class PROJECT_HUITZ_API UPauseMenuWidgetBase : public UWidgetBase {
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category="Pause Menu")
	UPauseMenuComponent* GetBackend() const;
	
	virtual TObjectPtr<UConfirmationDialogWidgetBase> CreateConfirmationDialog() const override;
};
