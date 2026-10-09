// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "PhysicalButton.generated.h"

UCLASS(Abstract)
class PROJECT_HUITZ_API APhysicalButton : public AInteractableObject {
	GENERATED_BODY()

public:
	APhysicalButton();
	
	virtual void Interact(UPlayerInteractionComponent* SourceComponent) override;
};
