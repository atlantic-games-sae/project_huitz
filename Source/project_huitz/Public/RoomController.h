// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "UObject/Object.h"
#include "RoomController.generated.h"

class AEnemySpawner;

UCLASS()
class PROJECT_HUITZ_API ARoomController : public AActor, public IReactToTrigger {
	GENERATED_BODY()
	
public:
	ARoomController();
	
	virtual void OnTrigger_Implementation() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<AEnemySpawner>> EnemySpawners;
};
