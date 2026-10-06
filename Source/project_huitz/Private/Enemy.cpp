// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::TakeDamage(float DamageTaken) {
	CurrentHealth -= DamageTaken;
	
	if (CurrentHealth <= 0) {
		Destroy();
		OnDeath();
	}
	
	OnDamageTaken(DamageTaken);
}
