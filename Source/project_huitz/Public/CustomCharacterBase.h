// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Libraries/DelegateLibrary.h"
#include "CustomCharacterBase.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API ACustomCharacterBase : public ACharacter {
	GENERATED_BODY()

public:
	ACustomCharacterBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float DamageTaken);

	FOnFloatValueChangedSignature OnCurrentHealthChangedDelegate;
	FOnFloatValueChangedSignature OnMaxHealthChangedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void Kill();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(float DamageTaken);

	virtual void OnDeath();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnDeath")
	void OnDeath_Event();
};
