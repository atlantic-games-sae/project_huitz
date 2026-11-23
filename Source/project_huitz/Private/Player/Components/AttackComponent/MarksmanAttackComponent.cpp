// © 2026 Atlantic Games. All rights reserved.

#include "Player/Components/AttackComponent/MarksmanAttackComponent.h"

#include "Libraries/HelperFunctions.h"
#include "Camera/CameraComponent.h"
#include "Enemies/BaseEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

void UMarksmanAttackComponent::GiveNewWeapon(TSubclassOf<URangedWeapon> RangedWeapon, bool bShouldEquipWeapon) {
	Super::GiveNewWeapon(RangedWeapon, bShouldEquipWeapon);
}

bool UMarksmanAttackComponent::BeginAttack() {
	if (!Super::BeginAttack()) return false;

	EDrawDebugTrace::Type DrawDebugTrace = bDebugAttackTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		UCameraComponent* Camera = GetOwner()->GetComponentByClass<UCameraComponent>();
		FVector TraceEnd = Camera->GetComponentLocation() + Camera->GetForwardVector() * ActiveWeapon->Range;
		FHitResult OutHit;
		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			this, Camera->GetComponentLocation(), TraceEnd,
			UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
			{}, DrawDebugTrace, OutHit, true);

		AsGun->AmmoRemainingInClip--;
		OnCurrentAmmoChangedDelegate.Broadcast(AsGun->AmmoRemainingInClip);

		if (AsGun->AmmoRemainingInClip == 0) BeginReload();

		if (!bHit) return true;

		if (ABaseEnemy* HitEnemy = Cast<ABaseEnemy>(OutHit.GetActor())) {
			float DamageToDeal = ActiveWeapon->DamagePerHit;
			if (AsGun->bHasDamageFalloff) {
				if (OutHit.Distance > AsGun->DamageFalloffInitialThreshold) {
					if (OutHit.Distance >= AsGun->DamageFalloffCompleteThreshold) {
						DamageToDeal *= AsGun->DamageFalloffMultiplier;
					} else {
						float DamageFalloffAlpha = UHelperFunctions::GetAlphaInRange(OutHit.Distance, AsGun->DamageFalloffInitialThreshold, AsGun->DamageFalloffCompleteThreshold);
						float LerpedFalloffMultiplier = FMath::Lerp(1.0f, AsGun->DamageFalloffMultiplier, DamageFalloffAlpha);
						DamageToDeal *= LerpedFalloffMultiplier;
					}
				}
			}

			HitEnemy->TakeDamage(DamageToDeal);
		}
	}

	return true;
}
