// © 2026 Atlantic Games. All rights reserved.

#include "RoomController.h"

#include "Components/BillboardComponent.h"
#include "Enemies/EnemySpawner.h"

void ARoomController::OnTrigger_Implementation() {
	for (TObjectPtr TargetSpawner : EnemySpawners) {
		Execute_OnTrigger(TargetSpawner);
	}
}
