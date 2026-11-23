// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterBase.h"
#include "BaseEnemy.generated.h"

UCLASS(NotBlueprintable, NotPlaceable)
class PROJECT_HUITZ_API ABaseEnemy : public ACustomCharacterBase {
	GENERATED_BODY()

public:
	ABaseEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void BeginAttack();

	bool bIsAttacking;

protected:
	UPROPERTY(Category="Custom Enemy Properties|Attack", EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugAttackTraces;

	UPROPERTY(Category="Custom Enemy Properties|Attack", EditDefaultsOnly, BlueprintReadOnly)
	float DamagePerHit;

	UPROPERTY(Category="Custom Enemy Properties|Attack", EditDefaultsOnly, BlueprintReadOnly)
	float AttackRange;

	UFUNCTION(BlueprintCallable)
	void FinishAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginAttack();

	UFUNCTION(BlueprintCallable)
	virtual void PerformAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPerformAttack();
};
