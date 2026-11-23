// © 2026 Atlantic Games. All rights reserved.

#include "Enemies/BaseEnemy.h"
#include "Enemies/EnemyAIController.h"

ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bDebugAttackTraces = false;
	DamagePerHit = 5.0f;
	AttackRange = 150.0f; // 1.5m

	bIsAttacking = false;
}

void ABaseEnemy::BeginAttack() {
	bIsAttacking = true;
	OnBeginAttack();
}

void ABaseEnemy::FinishAttack() {
	bIsAttacking = false;
}

void ABaseEnemy::PerformAttack() {
	OnPerformAttack();
}
