// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "MainMenuWidgetBase.generated.h"

class UCustomGameInstance;
class AMainMenuLevelScript;

UCLASS(Abstract)
class PROJECT_HUITZ_API UMainMenuWidgetBase : public UWidgetBase {
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, Category="Main Menu")
	AMainMenuLevelScript* GetBackend() const;
	
	virtual TObjectPtr<UConfirmationDialogWidgetBase> CreateConfirmationDialog() const override;
};
