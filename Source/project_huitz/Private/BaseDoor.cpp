// © 2026 Atlantic Games. All rights reserved.

#include "BaseDoor.h"

#include "Libraries/HelperFunctions.h"

ABaseDoor::ABaseDoor() {
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	DoorOpeningSpeed = 1.0f;
	bStartOpen = false;
}

void ABaseDoor::BeginPlay() {
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	
	if (bStartOpen) OpenDoor(true);
}

void ABaseDoor::MoveDoor() {
	SetActorLocation(FMath::Lerp(StartLocation, StartLocation + RelativeOpenLocation, DoorProgress));
	SetActorRotation(FMath::Lerp(StartRotation, StartRotation + RelativeOpenRotation, DoorProgress));
}

void ABaseDoor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	switch (DoorState) {
		case Opening:
			DoorProgress = UHelperFunctions::FloatMoveTowards(DoorProgress, 1.0f, DoorOpeningSpeed * DeltaTime);
			if (DoorProgress == 1.0f) DoorState = Open;
			MoveDoor();
			break;
		case Closing:
			DoorProgress = UHelperFunctions::FloatMoveTowards(DoorProgress, 0.0f, DoorOpeningSpeed * DeltaTime);
			if (DoorProgress == 0.0f) DoorState = Closed;
			MoveDoor();
			break;
		default:
			break;
	}
}

void ABaseDoor::OnTrigger_Implementation() {
	IReactToTrigger::OnTrigger_Implementation();
	ToggleDoor(false);
}

void ABaseDoor::OpenDoor(bool bShouldBeInstant) {
	if (bShouldBeInstant) {
		SetActorLocation(StartLocation + RelativeOpenLocation);
		SetActorRotation(StartRotation + RelativeOpenRotation);
		DoorState = Open;
	} else DoorState = Opening;
}

void ABaseDoor::CloseDoor(bool bShouldBeInstant) {
	if (bShouldBeInstant) {
		SetActorLocation(StartLocation);
		SetActorRotation(StartRotation);
		DoorState = Closed;
	} else DoorState = Closing;
}

void ABaseDoor::ToggleDoor(bool bShouldBeInstant) {
	switch (DoorState) {
		case Closed:
			OpenDoor(bShouldBeInstant);
			break;
		case Opening:
			if (bShouldBeInstant) OpenDoor(true);
			else CloseDoor(false);
			break;
		case Open:
			CloseDoor(bShouldBeInstant);
			break;
		case Closing:
			if (bShouldBeInstant) CloseDoor(true);
			else OpenDoor(false);
			break;
	}
}
