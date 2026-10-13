// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/Enemy.h"
#include "Enemies/EnemyAIController.h"

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

