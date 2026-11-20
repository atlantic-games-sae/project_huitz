// © 2026 Atlantic Games. All rights reserved.

#include "Player/Components/AttackComponent/BaseAttackComponent.h"

UBaseAttackComponent::UBaseAttackComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseAttackComponent::BeginPlay() {
	Super::BeginPlay();
	
	EquipWeapon(0);
}

bool UBaseAttackComponent::PerformAttack() {
	if (ActiveCooldown <= 0.0) {
		ActiveCooldown = ActiveWeapon->AttackCooldown;
		OnAttackCooldownUpdatedDelegate.Broadcast(ActiveCooldown);
		OnPerformAttack();
		return true;
	}
	return false;
}

void UBaseAttackComponent::PerformReload() {
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		ActiveCooldown = AsGun->TimeTakenToReload;
		OnAttackCooldownUpdatedDelegate.Broadcast(ActiveCooldown / ActiveWeapon->AttackCooldown);
		bIsReloading = true;
	}
}

void UBaseAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ActiveCooldown > 0.0f) {
		ActiveCooldown -= DeltaTime;
		OnAttackCooldownUpdatedDelegate.Broadcast(ActiveCooldown / ActiveWeapon->AttackCooldown);
	}
	
	if (!IsValid(ActiveWeapon)) return;
	
	if (ActiveCooldown <= 0.0f) {
		if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
			if (bIsReloading) {
				AsGun->AmmoRemainingInClip = AsGun->AmmoPerClip;
				OnCurrentAmmoChangedDelegate.Broadcast(AsGun->AmmoRemainingInClip);
				bIsReloading = false;
			}
			
			if (AsGun->bIsAutomatic && bIsPrimaryAttackInputHeld) PerformAttack();
		}
	}
}

void UBaseAttackComponent::GiveNewWeapon(TSubclassOf<UWeapon> NewWeapon, bool bShouldEquipWeapon) {
	if (OwnedWeapons.Contains(NewWeapon)) return;
	
	int WeaponIndex = OwnedWeapons.Add(NewWeapon);
	if (bShouldEquipWeapon) EquipWeapon(WeaponIndex);
}

void UBaseAttackComponent::EquipWeapon(int WeaponIndex) {
	if (WeaponIndex < 0 || WeaponIndex >= OwnedWeapons.Num()) return;
	
	ActiveWeapon = NewObject<UWeapon>(this, OwnedWeapons[WeaponIndex]);
	
	bool bDoesWeaponUseAmmo = false;
	
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		bDoesWeaponUseAmmo = true;
		OnMaxAmmoChangedDelegate.Broadcast(AsGun->AmmoPerClip);
		AsGun->AmmoRemainingInClip = AsGun->AmmoPerClip;
		OnCurrentAmmoChangedDelegate.Broadcast(AsGun->AmmoRemainingInClip);
	}
	
	OnActiveWeaponChangedDelegate.Broadcast(bDoesWeaponUseAmmo);
}

void UBaseAttackComponent::OnPrimaryAttackInputDown() {
	OnPrimaryAttackInputDown_Event();
	
	bIsPrimaryAttackInputHeld = true;
	
	if (!IsValid(ActiveWeapon) || ActiveCooldown > 0.0f) return;
	
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		if (AsGun->AmmoRemainingInClip > 0) PerformAttack();
		else PerformReload();
	} else PerformAttack();
}

void UBaseAttackComponent::OnPrimaryAttackInputReleased() {
	OnPrimaryAttackInputReleased_Event();
	
	bIsPrimaryAttackInputHeld = false;
}

void UBaseAttackComponent::OnReloadInputDown() {
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		if (AsGun->AmmoRemainingInClip < AsGun->AmmoPerClip) {
			PerformReload();
		}
	}
}
