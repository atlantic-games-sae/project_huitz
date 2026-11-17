// © 2026 Atlantic Games. All rights reserved.

#include "Enemies/EnemySpawner.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"

AEnemySpawner::AEnemySpawner() {
	UArrowComponent* ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName(TEXT("Arrow")));
	RootComponent = ArrowComponent;
}

void AEnemySpawner::SpawnEnemy() {
	if (!IsValid(EnemyType)) {
		UE_LOG(LogTemp, Error, TEXT("Variable EnemyType in EnemySpawner %s was invalid, returning"), *(this->GetName()));
		return;
	}

	float EnemyHalfHeight = EnemyType.GetDefaultObject()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyType, GetActorLocation() + FVector(0,0,EnemyHalfHeight), GetActorRotation(), SpawnParams);
	NewEnemy->OnPawnBeginPlay.AddUObject(this, &AEnemySpawner::OnEnemySpawned);
}

void AEnemySpawner::OnEnemySpawned(APawn* SpawnedPawn) {
	SpawnedPawn->OnDestroyed.AddUniqueDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
	AEnemy* AsEnemy = Cast<AEnemy>(SpawnedPawn);
	ActiveEnemies.AddUnique(AsEnemy);
	SpawnedPawn->OnPawnBeginPlay.RemoveAll(this);
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor) {
	if (AEnemy* AsEnemy = Cast<AEnemy>(DestroyedActor)) {
		ActiveEnemies.Remove(AsEnemy);
	}
}

void AEnemySpawner::OnTrigger_Implementation() {
	SpawnEnemy();
}
