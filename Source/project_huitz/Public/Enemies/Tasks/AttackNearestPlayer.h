// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/EnemyAIController.h"
#include "Player/PlayerCharacter.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AttackNearestPlayer.generated.h"

UCLASS()
class PROJECT_HUITZ_API UAttackNearestPlayer : public UStateTreeTaskBlueprintBase {
	GENERATED_BODY()

public:
	UAttackNearestPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
		bShouldCallTick = true;
	}

	UPROPERTY(EditAnywhere)
	TObjectPtr<ABaseEnemy> EnemyPawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AEnemyAIController> AIController;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override {
		TargetPlayer = nullptr;

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), OutActors);

		float DistanceToPlayer;
		TargetPlayer = Cast<APlayerCharacter>(UGameplayStatics::FindNearestActor(EnemyPawn->GetActorLocation(), OutActors, DistanceToPlayer));

		EnemyPawn->BeginAttack();

		return EStateTreeRunStatus::Running;
	}

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override {
		FRotator TargetLookRotation = UKismetMathLibrary::FindLookAtRotation(EnemyPawn->GetActorLocation(), TargetPlayer->GetActorLocation());
		EnemyPawn->SetActorRotation(FRotator(0, TargetLookRotation.Yaw, 0));

		return EnemyPawn->bIsAttacking ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
	}

private:
	FDelegateHandle PathFollowingDelegateHandle;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> TargetPlayer;

	EStateTreeRunStatus FallbackResult;
};
