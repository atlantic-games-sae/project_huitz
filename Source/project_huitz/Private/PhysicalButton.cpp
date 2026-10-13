// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicalButton.h"
#include "Interfaces/ReactToTriggerInterface.h"

APhysicalButton::APhysicalButton() {
	PrimaryActorTick.bCanEverTick = true;
}

void APhysicalButton::Interact(UPlayerInteractionComponent* SourceComponent) {
	Super::Interact(SourceComponent);
	
	for (TWeakObjectPtr Actor : ActorsToTrigger) {
		if (Actor->Implements<UReactToTrigger>()) {
			IReactToTrigger::Execute_OnTrigger(Actor.Get());
		}
	}
}
