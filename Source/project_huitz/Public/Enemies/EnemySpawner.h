// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "EnemySpawner.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API AEnemySpawner : public AActor, public IReactToTrigger {
	GENERATED_BODY()

public:
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseEnemy> EnemyType;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<ABaseEnemy>> ActiveEnemies;

	UFUNCTION()
	void OnEnemySpawned(APawn* SpawnedPawn);

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	virtual void OnTrigger_Implementation() override;
};
