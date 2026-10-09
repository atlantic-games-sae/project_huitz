// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class UPlayerInteractionComponent;

UCLASS(Abstract)
class PROJECT_HUITZ_API AInteractableObject : public AActor {
	GENERATED_BODY()
	
public:
	AInteractableObject();
	
	virtual void Interact(UPlayerInteractionComponent* SourceComponent);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(UPlayerInteractionComponent* SourceComponent);
	
	UPROPERTY(Category="Components", EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
};
