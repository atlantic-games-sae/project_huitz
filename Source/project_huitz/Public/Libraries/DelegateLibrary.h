// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DelegateLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedSignature, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntValueChangedSignature, int, Value);

UCLASS()
class PROJECT_HUITZ_API UDelegateLibrary : public UObject {
	GENERATED_BODY()
};
