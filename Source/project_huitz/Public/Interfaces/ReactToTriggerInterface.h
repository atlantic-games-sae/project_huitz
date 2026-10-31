// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReactToTriggerInterface.generated.h"

UINTERFACE()
class UReactToTrigger : public UInterface {
	GENERATED_BODY()
};

class PROJECT_HUITZ_API IReactToTrigger {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTrigger();
};
