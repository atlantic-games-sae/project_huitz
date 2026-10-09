// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicalButton.h"

APhysicalButton::APhysicalButton() {
	PrimaryActorTick.bCanEverTick = true;
}

void APhysicalButton::Interact(UPlayerInteractionComponent* SourceComponent) {
	Super::Interact(SourceComponent);
	
}
