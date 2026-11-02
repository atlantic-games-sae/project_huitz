// © 2026 Atlantic Games. All rights reserved.

#include "Enemies/EnemySpawner.h"
#include "Components/ArrowComponent.h"

AEnemySpawner::AEnemySpawner() {
	UArrowComponent* ArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName(TEXT("Arrow")));
	RootComponent = ArrowComponent;
}

void AEnemySpawner::SpawnEnemy() {
	if (!IsValid(EnemyType)) {
		UE_LOG(LogTemp, Error, TEXT("Variable EnemyType in EnemySpawner %s was invalid, returning"), *(this->GetName()));
		return;
	}
	
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyType, GetActorLocation(), GetActorRotation());
	NewEnemy->OnDestroyed.AddUniqueDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
	ActiveEnemies.AddUnique(NewEnemy);
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor) {
	if (AEnemy* AsEnemy = Cast<AEnemy>(DestroyedActor)) {
		ActiveEnemies.Remove(AsEnemy);
	}
}

void AEnemySpawner::OnTrigger_Implementation() {
	SpawnEnemy();
}
