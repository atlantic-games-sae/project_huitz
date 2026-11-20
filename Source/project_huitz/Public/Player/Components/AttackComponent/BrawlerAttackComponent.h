// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseAttackComponent.h"
#include "BrawlerAttackComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_HUITZ_API UBrawlerAttackComponent : public UBaseAttackComponent {
	GENERATED_BODY()

public:
	void GiveNewWeapon(TSubclassOf<UMeleeWeapon> MeleeWeapon, bool bShouldEquipWeapon);
	
protected:
	virtual bool PerformAttack() override;
};
