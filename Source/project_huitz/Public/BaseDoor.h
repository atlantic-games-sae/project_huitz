// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "BaseDoor.generated.h"

enum EDoorState : uint8 {
	Closed,
	Opening,
	Open,
	Closing
};

UCLASS(Abstract)
class PROJECT_HUITZ_API ABaseDoor : public AActor, public IReactToTrigger {
	GENERATED_BODY()

public:
	ABaseDoor();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnTrigger_Implementation() override;
	
	void OpenDoor(bool bShouldBeInstant);
	
	void CloseDoor(bool bShouldBeInstant);
	
	void ToggleDoor(bool bShouldBeInstant);

protected:
	virtual void BeginPlay() override;
	
	virtual void MoveDoor();
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector RelativeOpenLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RelativeOpenRotation;
	
	UPROPERTY(BlueprintReadOnly)
	FVector StartLocation;
	
	UPROPERTY(BlueprintReadOnly)
	FRotator StartRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DoorOpeningSpeed;
	
	UPROPERTY(BlueprintReadOnly)
	float DoorProgress;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartOpen;
	
private:
	EDoorState DoorState;
};
