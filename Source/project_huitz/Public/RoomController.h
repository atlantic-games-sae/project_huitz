// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "UObject/Object.h"
#include "RoomController.generated.h"

class AEnemySpawner;

USTRUCT(BlueprintType)
struct PROJECT_HUITZ_API FWave {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<TObjectPtr<AEnemySpawner>> SpawnersInWave;
};

UCLASS(Abstract)
class PROJECT_HUITZ_API ARoomController : public AActor, public IReactToTrigger {
	GENERATED_BODY()

public:
	ARoomController();
	
	virtual void OnTrigger_Implementation() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<FWave> Waves;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> PlayerSpawnAfterRoomComplete;
	
	UFUNCTION()
	void OnSpawnerAllEnemiesDestroyed();
	
protected:
	void BeginWave();
	
	virtual void OnWaveComplete();
	
	virtual void OnRoomComplete();
	
	int CurrentWaveIndex;
};
