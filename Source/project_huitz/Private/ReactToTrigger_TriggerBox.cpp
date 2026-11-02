// © 2026 Atlantic Games. All rights reserved.

#include "ReactToTrigger_TriggerBox.h"

#include "Components/ShapeComponent.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "Player/PlayerCharacter.h"


void AReactToTrigger_TriggerBox::BeginPlay() {
	Super::BeginPlay();
	GetCollisionComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &AReactToTrigger_TriggerBox::OnCollisionComponentBeginOverlap);
}

void AReactToTrigger_TriggerBox::OnCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(APlayerCharacter::StaticClass())) {
		IReactToTrigger::Execute_OnTrigger(ActorToTrigger);
		Destroy();
	}
}
