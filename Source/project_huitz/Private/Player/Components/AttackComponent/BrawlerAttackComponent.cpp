// © 2026 Atlantic Games. All rights reserved.

#include "Player/Components/AttackComponent/BrawlerAttackComponent.h"

#include "Camera/CameraComponent.h"
#include "Enemies/BaseEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Libraries/CollisionLibrary.h"

void UBrawlerAttackComponent::GiveNewWeapon(TSubclassOf<UMeleeWeapon> MeleeWeapon, bool bShouldEquipWeapon) {
	Super::GiveNewWeapon(MeleeWeapon, bShouldEquipWeapon);
}

bool UBrawlerAttackComponent::BeginAttack() {
	if (!Super::BeginAttack()) return false;
	EDrawDebugTrace::Type DrawDebugTrace = bDebugAttackTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (UMeleeWeapon* AsMeleeWeapon = Cast<UMeleeWeapon>(ActiveWeapon)) {
		UCameraComponent* Camera = GetOwner()->GetComponentByClass<UCameraComponent>();
		TArray<FHitResult> OutHits;
		bool bHit = UCollisionLibrary::ConeTraceMulti(
			this, Camera->GetComponentLocation(), Camera->GetComponentRotation(),
			AsMeleeWeapon->Range, AsMeleeWeapon->AttackConeHalfAngle,
			UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
			{GetOwner()}, DrawDebugTrace, OutHits);

		if (!bHit) return true;

		for (FHitResult& OutHit : OutHits) {
			if (ABaseEnemy* AsEnemy = Cast<ABaseEnemy>(OutHit.GetActor())) {
				AsEnemy->TakeDamage(AsMeleeWeapon->DamagePerHit);
			}
		}
	}

	return true;
}
