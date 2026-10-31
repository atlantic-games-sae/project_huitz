// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmationDialogWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConfirmationComplete, bool, bResult);

UCLASS(Abstract)
class PROJECT_HUITZ_API UConfirmationDialogWidgetBase : public UUserWidget {
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	UPROPERTY(BlueprintCallable)
	FConfirmationComplete OnConfirmationComplete;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetConfirmationMessage(const FString& MessageText);
	
private:
	UFUNCTION()
	void RemoveFromParentOnConfirmationComplete(bool bResult);
};
