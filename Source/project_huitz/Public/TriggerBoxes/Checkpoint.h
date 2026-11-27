// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Engine/TriggerBox.h"
#include "Checkpoint.generated.h"

UCLASS()
class PROJECT_HUITZ_API ACheckpoint : public ATriggerBox {
	GENERATED_BODY()
	
public:
	ACheckpoint();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;
	
	UPROPERTY(Category="Checkpoint", EditAnywhere, BlueprintReadOnly)
	bool bAutoApplyToBothPlayers; 
};
