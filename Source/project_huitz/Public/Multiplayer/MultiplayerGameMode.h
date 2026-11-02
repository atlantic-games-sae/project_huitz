// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameMode.h"
#include "MultiplayerGameMode.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECT_HUITZ_API AMultiplayerGameMode : public ACustomGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
