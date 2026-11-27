// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "AirLock.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECT_HUITZ_API AAirLock : public ATriggerBox {
	GENERATED_BODY()

public:
	AAirLock();
	
	UPROPERTY(Category="Air Lock", EditInstanceOnly)
	TSet<TObjectPtr<AActor>> ActorsToTrigger;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	TArray<TObjectPtr<APlayerCharacter>> PlayersCurrentlyInside;
	
	UPROPERTY(Category="Air Lock", EditAnywhere, BlueprintReadOnly)
	bool bActAsCheckpoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> CheckpointArrow;
};
