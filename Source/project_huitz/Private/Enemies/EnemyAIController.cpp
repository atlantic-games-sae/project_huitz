// © 2026 Atlantic Games. All rights reserved.

#include "Enemies/EnemyAIController.h"

AEnemyAIController::AEnemyAIController() {
	PrimaryActorTick.bCanEverTick = true;
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(FName(TEXT("StateTreeComp")));
	bStartAILogicOnPossess = true;
}
