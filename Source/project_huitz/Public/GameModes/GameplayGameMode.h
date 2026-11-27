// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameMode.h"
#include "GameplayGameMode.generated.h"

UCLASS()
class PROJECT_HUITZ_API AGameplayGameMode : public ACustomGameMode {
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsInCombat;
};
