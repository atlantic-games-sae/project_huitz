// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECT_HUITZ_API AEnemy : public ACharacter {
	GENERATED_BODY()

public:
	AEnemy();
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float DamageTaken);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(float DamageTaken);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 25.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth = MaxHealth;
};
