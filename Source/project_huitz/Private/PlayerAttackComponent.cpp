// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAttackComponent.h"
#include "Enemy.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerAttackComponent::UPlayerAttackComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerAttackComponent::BeginPlay() {
	Super::BeginPlay();
	
	EquipWeapon(0);
}

void UPlayerAttackComponent::PerformAttack() {
	OnPerformAttack();
	
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		FVector TraceEnd = GetPlayerCharacter()->Camera->GetForwardVector() * ActiveWeapon->Range;
		FHitResult OutHit;
		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			this, GetPlayerCharacter()->Camera->GetComponentLocation(), TraceEnd,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			{}, EDrawDebugTrace::ForDuration, OutHit, true);
		
		AsGun->AmmoRemainingInClip--;
		OnCurrentAmmoChangedDelegate.Broadcast(AsGun->AmmoRemainingInClip);
		
		if (AsGun->AmmoRemainingInClip == 0) PerformReload();
		
		if (!bHit) return;
		
		if (AEnemy* HitEnemy = Cast<AEnemy>(OutHit.GetActor())) {
			HitEnemy->TakeDamage(ActiveWeapon->DamagePerHit);
		}
	}
	
	if (ActiveCooldown <= 0.0) {
		ActiveCooldown = ActiveWeapon->AttackCooldown;
	}
}

void UPlayerAttackComponent::PerformReload() {
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		ActiveCooldown = AsGun->TimeTakenToReload;
		AsGun->bIsReloading = true;
	}
}

void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ActiveCooldown > 0.0f) ActiveCooldown -= DeltaTime;
	
	if (ActiveWeapon == nullptr) return;
	
	if (ActiveCooldown <= 0.0f) {
		if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
			if (AsGun->bIsReloading) {
				AsGun->AmmoRemainingInClip = AsGun->AmmoPerClip;
				OnCurrentAmmoChangedDelegate.Broadcast(AsGun->AmmoRemainingInClip);
				AsGun->bIsReloading = false;
			}
			
			if (AsGun->bIsAutomatic && bIsPrimaryAttackInputHeld) PerformAttack();
		}
	}
}

void UPlayerAttackComponent::GiveNewWeapon(TSubclassOf<UWeapon> NewWeapon, bool bShouldEquipWeapon) {
	if (OwnedWeapons.Contains(NewWeapon)) return;
	
	OwnedWeapons.Add(NewWeapon);
	if (bShouldEquipWeapon) EquipWeapon(OwnedWeapons.Num() - 1);
}

void UPlayerAttackComponent::EquipWeapon(int WeaponIndex) {
	if (WeaponIndex < 0 || WeaponIndex >= OwnedWeapons.Num()) return;
	
	ActiveWeapon = NewObject<UWeapon>(this, OwnedWeapons[WeaponIndex]);
	
	bool bDoesWeaponUseAmmo = false;
	
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		bDoesWeaponUseAmmo = true;
		OnMaxAmmoChangedDelegate.Broadcast(AsGun->AmmoPerClip);
	}
	
	OnActiveWeaponChangedDelegate.Broadcast(bDoesWeaponUseAmmo);
}

void UPlayerAttackComponent::OnPrimaryAttackInputDown() {
	OnPrimaryAttackInputDown_Event();
	
	bIsPrimaryAttackInputHeld = true;
	
	if (ActiveWeapon == nullptr || ActiveCooldown > 0.0f) return;
	
	if (UGun* AsGun = Cast<UGun>(ActiveWeapon)) {
		if (AsGun->AmmoRemainingInClip > 0) PerformAttack();
		else PerformReload();
	} else PerformAttack();
}

void UPlayerAttackComponent::OnPrimaryAttackInputReleased() {
	OnPrimaryAttackInputReleased_Event();
	
	bIsPrimaryAttackInputHeld = false;
}
