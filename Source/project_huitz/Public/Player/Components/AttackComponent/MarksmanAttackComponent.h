// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "MarksmanAttackComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_HUITZ_API UMarksmanAttackComponent : public UBaseAttackComponent {
	GENERATED_BODY()

public:
	void GiveNewWeapon(TSubclassOf<URangedWeapon> RangedWeapon, bool bShouldEquipWeapon);

protected:
	virtual bool BeginAttack() override;
};
