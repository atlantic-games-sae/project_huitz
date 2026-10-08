// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API AEnemySpawner : public AActor {
	GENERATED_BODY()
	
public:
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> EnemyType;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AEnemy*> ActiveEnemies;
	
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
};
