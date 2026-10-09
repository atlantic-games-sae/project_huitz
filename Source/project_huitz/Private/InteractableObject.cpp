// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "PlayerInteractionComponent.h"

AInteractableObject::AInteractableObject() {
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh")));
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AInteractableObject::Interact(UPlayerInteractionComponent* SourceComponent) {
	OnInteract(SourceComponent);
}
