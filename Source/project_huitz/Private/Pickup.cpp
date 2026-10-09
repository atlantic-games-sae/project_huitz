// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "PlayerCharacter.h"

APickup::APickup() {
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Trigger Box")));
}

void APickup::BeginPlay() {
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &APickup::OnBeginOverlap);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (APlayerCharacter* AsPlayer = Cast<APlayerCharacter>(OtherActor)) {
		GivePlayerPickup(AsPlayer);
	}
}

void APickup::GivePlayerPickup(APlayerCharacter* Player) {
	GivePlayerPickup_Event(Player);
}
