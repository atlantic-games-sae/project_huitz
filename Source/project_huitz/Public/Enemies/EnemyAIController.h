// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "EnemyAIController.generated.h"

UCLASS()
class PROJECT_HUITZ_API AEnemyAIController : public AAIController {
	GENERATED_BODY()

public:
	AEnemyAIController();
	
protected:
	UPROPERTY(Category="Components", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStateTreeAIComponent> StateTreeAI;
};
