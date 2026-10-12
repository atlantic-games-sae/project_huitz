// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCharacterBase.h"
#include "Components/CapsuleComponent.h"

ACustomCharacterBase::ACustomCharacterBase(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	MaxHealth = 50.0f;
	CurrentHealth = MaxHealth;
}

void ACustomCharacterBase::TakeDamage(float DamageTaken) {
	CurrentHealth -= DamageTaken;
	OnCurrentHealthChangedDelegate.Broadcast(CurrentHealth);
	
	if (CurrentHealth <= 0) {
		OnDeath();
	}
	
	OnDamageTaken(DamageTaken);
}

void ACustomCharacterBase::OnDeath() {
	OnDeath_Event();
}
