// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS()
class PROJECT_HUITZ_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Initialize(float pCurrentHealth, float pMaxHealth);
	
	UFUNCTION()
	void OnCurrentHealthChanged(float Value);
	
	UFUNCTION()
	void OnMaxHealthChanged(float Value);
	
	UFUNCTION()
	void OnCurrentAmmoChanged(int Value);
	
	UFUNCTION()
	void OnMaxAmmoChanged(int Value);
	
	UFUNCTION()
	void OnActiveWeaponChanged(bool bDoesWeaponUseAmmo);
	
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
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnCurrentAmmoChanged")
	void OnCurrentAmmoChanged_Event();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnMaxAmmoChanged")
	void OnMaxAmmoChanged_Event();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnActiveWeaponChanged")
	void OnActiveWeaponChanged_Event();
};
