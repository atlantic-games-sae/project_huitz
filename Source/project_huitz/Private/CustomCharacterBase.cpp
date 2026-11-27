// © 2026 Atlantic Games. All rights reserved.

#include "CustomCharacterBase.h"
#include "Components/CapsuleComponent.h"

ACustomCharacterBase::ACustomCharacterBase(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	MaxHealth = 50.0f;
}

void ACustomCharacterBase::BeginPlay() {
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void ACustomCharacterBase::TakeDamage(float DamageTaken) {
	CurrentHealth -= DamageTaken;
	OnCurrentHealthChangedDelegate.Broadcast(CurrentHealth);

	if (CurrentHealth <= 0) {
		OnDeath();
	}

	OnDamageTaken(DamageTaken);
}

void ACustomCharacterBase::OnDeath() {
	OnDeath_Event();
}

void ACustomCharacterBase::Kill() {
	TakeDamage(CurrentHealth);
}
