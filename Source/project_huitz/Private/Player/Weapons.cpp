// © 2026 Atlantic Games. All rights reserved.

#include "Player/Weapons.h"

UWeapon::UWeapon() {
	PrettyName = "Placeholder Weapon Name";
	DamagePerHit = 25.0f;
	Range = 25000.0f; // 250m
	AttackCooldown = 1.0f; // 1 second
}

UMeleeWeapon::UMeleeWeapon() {
	AttackConeHalfAngle = 30.0f; // 30 degrees
}

URangedWeapon::URangedWeapon() {
	bHasDamageFalloff = true;
	DamageFalloffInitialThreshold = 5000.0f; // 50m
	DamageFalloffCompleteThreshold = 15000.0f; // 150m
	DamageFalloffMultiplier = 0.5f;
}

UGun::UGun() {
	AmmoPerClip = 12;
	bIsAutomatic = false;
	TimeTakenToReload = 1.0f;
}
