// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Components/ArrowComponent.h"

AEnemySpawner::AEnemySpawner() {
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<UArrowComponent>(FName(TEXT("Arrow")));
}

void AEnemySpawner::SpawnEnemy() {
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyType, GetActorLocation(), GetActorRotation());
	NewEnemy->OnDestroyed.AddUniqueDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
	ActiveEnemies.Add(NewEnemy);
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor) {
	if (AEnemy* AsEnemy = Cast<AEnemy>(DestroyedActor)) {
		ActiveEnemies.Remove(AsEnemy);
	}
}
