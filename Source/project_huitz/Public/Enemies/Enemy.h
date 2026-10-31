// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterBase.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECT_HUITZ_API AEnemy : public ACustomCharacterBase {
	GENERATED_BODY()
	
public:
	AEnemy(const FObjectInitializer& ObjectInitializer);
};
