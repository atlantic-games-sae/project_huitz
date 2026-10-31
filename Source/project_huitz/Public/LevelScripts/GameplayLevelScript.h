// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameplayLevelScript.generated.h"

struct FLevelSaveData;
class UCustomGameInstance;

UCLASS()
class PROJECT_HUITZ_API AGameplayLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadFromLevelSaveData(FLevelSaveData& SaveData);
};
