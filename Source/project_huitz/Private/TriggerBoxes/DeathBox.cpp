// © 2026 Atlantic Games. All rights reserved.

#include "TriggerBoxes/DeathBox.h"

#include "CustomCharacterBase.h"


void ADeathBox::NotifyActorBeginOverlap(AActor* OtherActor) {
	if (ACustomCharacterBase* AsCustomCharacter = Cast<ACustomCharacterBase>(OtherActor)) {
		AsCustomCharacter->Kill();
	}
}
