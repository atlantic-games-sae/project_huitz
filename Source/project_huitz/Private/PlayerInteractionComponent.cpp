// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInteractionComponent.h"

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

	if (bCanInteract) TargetInteractableObject = PerformInteractionTrace();
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
	if (TargetInteractableObject == nullptr) return;
	
	TargetInteractableObject->Interact(this);
}
