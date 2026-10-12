// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomController.h"

#include "EnemySpawner.h"

ARoomController::ARoomController() {
	CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
}

void ARoomController::OnTrigger_Implementation() {
	for (AEnemySpawner* TargetSpawner : EnemySpawners) {
		Execute_OnTrigger(TargetSpawner);
	}
}
