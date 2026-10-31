// © 2026 Atlantic Games. All rights reserved.

#include "InteractableObject.h"

AInteractableObject::AInteractableObject() {
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh")));
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AInteractableObject::Interact(UPlayerInteractionComponent* SourceComponent) {
	OnInteract(SourceComponent);
}
