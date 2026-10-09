// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

class AInteractableObject;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_HUITZ_API UPlayerInteractionComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void OnInteractInputDown();
	
	UPROPERTY(BlueprintReadWrite)
	bool bCanInteract;
	
protected:
	UPROPERTY(EditAnywhere, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float InteractionTraceRange;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float InteractionTraceRadius;
	
	UPROPERTY(EditAnywhere)
	bool bDebugInteractTrace;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AInteractableObject> TargetInteractableObject;
	
private:
	TObjectPtr<AInteractableObject> PerformInteractionTrace() const;
};
