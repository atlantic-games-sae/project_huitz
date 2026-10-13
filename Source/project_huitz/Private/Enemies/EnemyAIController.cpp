// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemies/EnemyAIController.h"

AEnemyAIController::AEnemyAIController() {
	PrimaryActorTick.bCanEverTick = true;
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(FName(TEXT("StateTreeComp")));
	bStartAILogicOnPossess = true;
}
