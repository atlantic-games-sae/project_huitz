// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "MeleeEnemy.generated.h"

UCLASS(Blueprintable)
class PROJECT_HUITZ_API AMeleeEnemy : public ABaseEnemy {
	GENERATED_BODY()

public:
	AMeleeEnemy(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category="Custom Enemy Properties|AI", EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0, ClampMin=0))
	float ChaseStopDistance;

protected:
	virtual void PerformAttack() override;

	UPROPERTY(Category="Custom Enemy Properties|Attack", EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin=0, ClampMin=0, UIMax=90, ClampMax=90))
	float AttackConeHalfAngle;
};
