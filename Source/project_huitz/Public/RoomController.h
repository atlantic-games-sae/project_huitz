// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "UObject/Object.h"
#include "RoomController.generated.h"

class AEnemySpawner;

UCLASS()
class PROJECT_HUITZ_API ARoomController : public AActor, public IReactToTrigger {
	GENERATED_BODY()
	
public:
	ARoomController();
	
	virtual void OnTrigger_Implementation() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<AEnemySpawner>> EnemySpawners;
};
