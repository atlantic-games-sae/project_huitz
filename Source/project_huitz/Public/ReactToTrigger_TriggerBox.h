// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ReactToTrigger_TriggerBox.generated.h"

UCLASS()
class PROJECT_HUITZ_API AReactToTrigger_TriggerBox : public ATriggerBox {
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<AActor> ActorToTrigger;
	
private:
	UFUNCTION()
	void OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
