// © 2026 Atlantic Games. All rights reserved.


#include "TriggerBoxes/AirLock.h"

#include "Interfaces/ReactToTriggerInterface.h"
#include "Player/PlayerCharacter.h"

AAirLock::AAirLock() {
	bActAsCheckpoint = true;
	
	CheckpointArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CheckpointArrow"));
	CheckpointArrow->SetupAttachment(RootComponent);
}

void AAirLock::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	
	{
		APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OtherActor);
		if (!IsValid(AsPlayer)) return;
		
		PlayersCurrentlyInside.AddUnique(AsPlayer);
	}
	
	if (PlayersCurrentlyInside.Num() >= 2) {
		if (bActAsCheckpoint) {
			for (APlayerCharacter* Player : PlayersCurrentlyInside) {
				Player->RegisterCheckpoint(CheckpointArrow->GetComponentLocation(), CheckpointArrow->GetComponentRotation().Yaw);
			}
		}
		
		for (AActor* ActorToTrigger : ActorsToTrigger) {
			IReactToTrigger::Execute_OnTrigger(ActorToTrigger);
		}
		
		Destroy();
	}
}

void AAirLock::NotifyActorEndOverlap(AActor* OtherActor) {
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OtherActor)) {
		PlayersCurrentlyInside.Remove(AsPlayer);
	}
}
