// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUD.h"

void UPlayerHUD::Initialize(float pCurrentHealth, float pMaxHealth) {
	CurrentHealth = pCurrentHealth;
	MaxHealth = pMaxHealth;
}

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

void UPlayerHUD::OnActiveWeaponChanged(bool bDoesWeaponUseAmmo) {
	bShouldDisplayAmmo = bDoesWeaponUseAmmo;
	
	OnActiveWeaponChanged_Event();
}
