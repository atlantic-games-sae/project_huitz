// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemies/EnemyAIController.h"
#include "Player/PlayerCharacter.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "ChaseNearestPlayer.generated.h"

UCLASS()
class PROJECT_HUITZ_API UChaseNearestPlayer : public UStateTreeTaskBlueprintBase {
	GENERATED_BODY()
	
public:
	UChaseNearestPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
		bShouldCallTick = true; 
	}
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AEnemy> EnemyPawn;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AEnemyAIController> AIController;
	
	UPROPERTY(EditAnywhere)
	float StopRange;
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<APlayerCharacter>> AllPlayers;
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override {
		TargetPlayer = nullptr;
		FallbackResult = EStateTreeRunStatus::Running;
		
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), OutActors);
		for (AActor* Actor : OutActors) {
			AllPlayers.AddUnique(Cast<APlayerCharacter>(Actor));
		}
		
		PathFollowingDelegateHandle = AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UChaseNearestPlayer::OnPathfindingRequestFinished);
		
		return PerformTargetSelectionLogic();
	}
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override {
		EStateTreeRunStatus Result = PerformTargetSelectionLogic();
		if (Result != EStateTreeRunStatus::Running) return Result;
		
		return FallbackResult;
	}
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override {
		AIController->GetPathFollowingComponent()->OnRequestFinished.Remove(PathFollowingDelegateHandle);
	}
	
private:
	FDelegateHandle PathFollowingDelegateHandle;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> TargetPlayer;
	
	EStateTreeRunStatus FallbackResult;
	
	void OnPathfindingRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result) {
		if (Result.IsSuccess()) {
			FallbackResult = EStateTreeRunStatus::Succeeded;
		} else if (Result.IsFailure()) {
			FallbackResult = EStateTreeRunStatus::Failed;
		} else if (Result.IsInterrupted()) {
			FallbackResult = EStateTreeRunStatus::Running;
		}
	}
	
	EStateTreeRunStatus PerformTargetSelectionLogic() {
		bool bHasChangedTarget = false;
		for (APlayerCharacter* Player : AllPlayers) {
			if (TargetPlayer == nullptr) {
				TargetPlayer = Player;
				bHasChangedTarget = true;
				continue;
			}
			if (Player == TargetPlayer) continue;
			float TargetDistance = (TargetPlayer->GetActorLocation() - EnemyPawn->GetActorLocation()).Length();
			float TestDistance = (Player->GetActorLocation() - EnemyPawn->GetActorLocation()).Length();
			if (TestDistance < TargetDistance) {
				TargetPlayer = Player;
				bHasChangedTarget = true;
			}
		}
		
		if (!bHasChangedTarget) return EStateTreeRunStatus::Running;
		
		switch (AIController->MoveToActor(TargetPlayer, StopRange)) {
			case EPathFollowingRequestResult::AlreadyAtGoal:
				return EStateTreeRunStatus::Succeeded;
			case EPathFollowingRequestResult::RequestSuccessful:
				return EStateTreeRunStatus::Running;
			case EPathFollowingRequestResult::Failed:
				return EStateTreeRunStatus::Failed;
			default:
				return EStateTreeRunStatus::Running;
		}
	}
};
