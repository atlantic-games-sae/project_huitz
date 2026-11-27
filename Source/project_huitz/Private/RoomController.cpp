// © 2026 Atlantic Games. All rights reserved.

#include "RoomController.h"

#include "Enemies/EnemySpawner.h"
#include "Libraries/HelperFunctions.h"

ARoomController::ARoomController() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	PlayerSpawnAfterRoomComplete = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerSpawn"));
	PlayerSpawnAfterRoomComplete->SetupAttachment(RootComponent);
	
	CurrentWaveIndex = 0;
}

void ARoomController::OnTrigger_Implementation() {
	UHelperFunctions::GetCustomGameplayGameMode(this)->bIsInCombat = true;
	
	BeginWave();
}

void ARoomController::OnSpawnerAllEnemiesDestroyed() {
	for (TObjectPtr TargetSpawner : Waves[CurrentWaveIndex].SpawnersInWave) {
		if (TargetSpawner->ActiveEnemies.Num() > 0) {
			return;
		}
	}
	
	OnWaveComplete();
}

void ARoomController::BeginWave() {
	for (TObjectPtr TargetSpawner : Waves[CurrentWaveIndex].SpawnersInWave) {
		TargetSpawner->OnAllEnemiesDestroyedDelegate.AddUniqueDynamic(this, &ARoomController::OnSpawnerAllEnemiesDestroyed);
		Execute_OnTrigger(TargetSpawner);
	}
}

void ARoomController::OnWaveComplete() {
	for (TObjectPtr TargetSpawner : Waves[CurrentWaveIndex].SpawnersInWave) {
		TargetSpawner->OnAllEnemiesDestroyedDelegate.RemoveDynamic(this, &ARoomController::OnSpawnerAllEnemiesDestroyed);
	}
	
	if (CurrentWaveIndex + 1 < Waves.Num()) {
		CurrentWaveIndex++;
		BeginWave();
	} else {
		OnRoomComplete();
	}
}

void ARoomController::OnRoomComplete() {
	UHelperFunctions::GetCustomGameplayGameMode(this)->bIsInCombat = false;
	UHelperFunctions::GetCustomGameInstance(this)->RegisterCheckpointForAllPlayers(
		PlayerSpawnAfterRoomComplete->GetComponentLocation(),
		PlayerSpawnAfterRoomComplete->GetComponentRotation().Yaw);
	UHelperFunctions::GetCustomGameInstance(this)->SpawnPlayerCharacters();
	Destroy();
}
