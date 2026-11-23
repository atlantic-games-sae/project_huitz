// © 2026 Atlantic Games. All rights reserved.


#include "Enemies/MeleeEnemy.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/CollisionLibrary.h"
#include "Player/PlayerCharacter.h"

AMeleeEnemy::AMeleeEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	AttackRange = 125.0f; // 1.25m

	ChaseStopDistance = 50.0f; // 0.5m
	AttackConeHalfAngle = 30.0f; // 30 degrees
}

void AMeleeEnemy::PerformAttack() {
	EDrawDebugTrace::Type DrawDebugTrace = bDebugAttackTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	TArray<FHitResult> OutHits;
	bool bHit = UCollisionLibrary::ConeTraceMulti(
		this, GetActorLocation(), GetActorRotation(),
		AttackRange, AttackConeHalfAngle,
		UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
		{GetOwner()}, DrawDebugTrace, OutHits);

	if (bHit) {
		for (FHitResult& OutHit : OutHits) {
			if (APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OutHit.GetActor())) {
				AsPlayer->TakeDamage(DamagePerHit);
			}
		}
	}

	Super::PerformAttack();
}
