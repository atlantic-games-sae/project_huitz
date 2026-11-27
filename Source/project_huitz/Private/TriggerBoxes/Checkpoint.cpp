// © 2026 Atlantic Games. All rights reserved.

#include "TriggerBoxes/Checkpoint.h"

#include "Libraries/HelperFunctions.h"
#include "Player/PlayerCharacter.h"

ACheckpoint::ACheckpoint() {
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("CheckpointArrow"));
	ArrowComponent->SetupAttachment(RootComponent);
	
	bAutoApplyToBothPlayers = false;
}

void ACheckpoint::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (bAutoApplyToBothPlayers) {
		UHelperFunctions::GetCustomGameInstance(this)->RegisterCheckpointForAllPlayers(
			ArrowComponent->GetComponentLocation(),
			ArrowComponent->GetComponentRotation().Yaw);
	} else if (APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OtherActor)) {
		AsPlayer->RegisterCheckpoint(ArrowComponent->GetComponentLocation(), ArrowComponent->GetComponentRotation().Yaw);
	}
}
