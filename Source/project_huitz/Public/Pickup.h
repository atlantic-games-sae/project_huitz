// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class APlayerCharacter;

UCLASS(Abstract)
class PROJECT_HUITZ_API APickup : public AActor {
	GENERATED_BODY()

public:
	APickup();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* TriggerBox;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void GivePlayerPickup(APlayerCharacter* Player);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="GivePlayerPickup")
	void GivePlayerPickup_Event(APlayerCharacter* Player);
};
