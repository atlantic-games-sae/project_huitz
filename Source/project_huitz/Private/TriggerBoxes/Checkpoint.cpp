// © 2026 Atlantic Games. All rights reserved.

#include "TriggerBoxes/Checkpoint.h"

#include "Player/PlayerCharacter.h"

ACheckpoint::ACheckpoint() {
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("CheckpointArrow"));
	ArrowComponent->SetupAttachment(RootComponent);
}

void ACheckpoint::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OtherActor)) {
		AsPlayer->RegisterCheckpoint(ArrowComponent->GetComponentLocation(), ArrowComponent->GetComponentRotation().Yaw);
	}
}
