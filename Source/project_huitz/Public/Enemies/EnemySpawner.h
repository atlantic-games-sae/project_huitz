// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PROJECT_HUITZ_API AEnemySpawner : public AActor, public IReactToTrigger {
	GENERATED_BODY()
	
public:
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> EnemyType;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<AEnemy>> ActiveEnemies;
	
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	virtual void OnTrigger_Implementation() override;
};
