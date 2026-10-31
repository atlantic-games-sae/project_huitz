// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedSignature, float, Value);

UCLASS(Abstract)
class PROJECT_HUITZ_API ACustomCharacterBase : public ACharacter {
	GENERATED_BODY()

public:
	ACustomCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float DamageTaken);
	
	FOnFloatValueChangedSignature OnCurrentHealthChangedDelegate;
	FOnFloatValueChangedSignature OnMaxHealthChangedDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(float DamageTaken);
	
	void OnDeath();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnDeath")
	void OnDeath_Event();
};
