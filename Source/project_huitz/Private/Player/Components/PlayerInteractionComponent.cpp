// © 2026 Atlantic Games. All rights reserved.

#include "Player/Components/PlayerInteractionComponent.h"
#include "InteractableObject.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerInteractionComponent::UPlayerInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	bCanInteract = true;
	
	InteractionTraceRange = 200.0f; // 2m
	InteractionTraceRadius = 10.0f; // 10cm
}

void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bCanInteract) {
		AInteractableObject* NewTargetInteractable = PerformInteractionTrace();
		if (NewTargetInteractable != TargetInteractableObject) {
			if (IsValid(TargetInteractableObject)) TargetInteractableObject->StaticMeshComponent->SetOverlayMaterial(nullptr);
			TargetInteractableObject = NewTargetInteractable;
			if (IsValid(TargetInteractableObject)) TargetInteractableObject->StaticMeshComponent->SetOverlayMaterial(TargetHighlightMaterial);
		}
	}
}

TObjectPtr<AInteractableObject> UPlayerInteractionComponent::PerformInteractionTrace() const {
	UCameraComponent* Camera = GetOwner()->GetComponentByClass<UCameraComponent>();
	FVector TraceEnd = Camera->GetForwardVector() * InteractionTraceRange + Camera->GetComponentLocation();
	EDrawDebugTrace::Type DrawDebugTrace = bDebugInteractTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	
	FHitResult OutHit;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		this, Camera->GetComponentLocation(), TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, {}, DrawDebugTrace, OutHit, true);
	
	if (!bHit) return nullptr;
	
	return Cast<AInteractableObject>(OutHit.GetActor());
}

void UPlayerInteractionComponent::OnInteractInputDown() {
	if (!IsValid(TargetInteractableObject)) return;
	
	TargetInteractableObject->Interact(this);
}
