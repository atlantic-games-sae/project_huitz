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

	ABaseEnemy* NewEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyType, GetActorLocation() + FVector(0, 0, EnemyHalfHeight), GetActorRotation());
	
	if (!IsValid(NewEnemy)) return;
	ActiveEnemies.AddUnique(NewEnemy);
	
	NewEnemy->OnDestroyed.AddUniqueDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor) {
	if (ABaseEnemy* AsEnemy = Cast<ABaseEnemy>(DestroyedActor)) {
		ActiveEnemies.Remove(AsEnemy);
		
		if (ActiveEnemies.Num() == 0) OnAllEnemiesDestroyedDelegate.Broadcast();
	}
}

void AEnemySpawner::OnTrigger_Implementation() {
	SpawnEnemy();
}
