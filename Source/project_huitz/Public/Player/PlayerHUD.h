// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API UPlayerHUD : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnCurrentHealthChanged(float Value);

	UFUNCTION()
	void OnMaxHealthChanged(float Value);

	UFUNCTION()
	void OnCurrentAmmoChanged(int Value);

	UFUNCTION()
	void OnMaxAmmoChanged(int Value);

	UFUNCTION()
	void OnActiveWeaponChanged(bool bNewShouldDisplayAmmo);

	UFUNCTION()
	void OnAttackCooldownUpdated(float Value);

protected:
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnCurrentHealthChanged")
	void OnCurrentHealthChanged_Event();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnMaxHealthChanged")
	void OnMaxHealthChanged_Event();

	UPROPERTY(BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(BlueprintReadOnly)
	int MaxAmmo;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldDisplayAmmo;

	UPROPERTY(BlueprintReadOnly)
	float AttackCooldownProgress;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnCurrentAmmoChanged")
	void OnCurrentAmmoChanged_Event();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnMaxAmmoChanged")
	void OnMaxAmmoChanged_Event();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnActiveWeaponChanged")
	void OnActiveWeaponChanged_Event();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnAttackCooldownUpdated")
	void OnAttackCooldownUpdated_Event();
};
