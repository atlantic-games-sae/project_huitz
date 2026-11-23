// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DeathBox.generated.h"

class ACustomCharacterBase;

UCLASS()
class PROJECT_HUITZ_API ADeathBox : public ATriggerBox {
	GENERATED_BODY()
	
public:
	ADeathBox();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACustomCharacterBase> TargetCharacterClass;
};
