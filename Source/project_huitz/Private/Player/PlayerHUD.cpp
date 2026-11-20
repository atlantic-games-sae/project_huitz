// © 2026 Atlantic Games. All rights reserved.

#include "Player/PlayerHUD.h"

void UPlayerHUD::OnCurrentHealthChanged(float Value) {
	CurrentHealth = Value;
	
	OnCurrentHealthChanged_Event();
}

void UPlayerHUD::OnMaxHealthChanged(float Value) {
	MaxHealth = Value;
	
	OnMaxHealthChanged_Event();
}

void UPlayerHUD::OnCurrentAmmoChanged(int Value) {
	CurrentAmmo = Value;
	
	OnCurrentAmmoChanged_Event();
}

void UPlayerHUD::OnMaxAmmoChanged(int Value) {
	MaxAmmo = Value;
	
	OnMaxAmmoChanged_Event();
}

void UPlayerHUD::OnActiveWeaponChanged(bool bNewShouldDisplayAmmo) {
	bShouldDisplayAmmo = bNewShouldDisplayAmmo;
	
	OnActiveWeaponChanged_Event();
}

void UPlayerHUD::OnAttackCooldownUpdated(float Value) {
	AttackCooldownProgress = Value;
	
	OnAttackCooldownUpdated_Event();
}
