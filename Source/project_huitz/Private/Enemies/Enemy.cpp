// © 2026 Atlantic Games. All rights reserved.

#include "Enemies/Enemy.h"
#include "Enemies/EnemyAIController.h"

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

