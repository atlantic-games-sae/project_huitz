// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class UPlayerInteractionComponent;

UCLASS(Abstract)
class PROJECT_HUITZ_API AInteractableObject : public AActor {
	GENERATED_BODY()
	
public:
	AInteractableObject();
	
	virtual void Interact(UPlayerInteractionComponent* SourceComponent);
	
	UPROPERTY(Category="Components", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(UPlayerInteractionComponent* SourceComponent);
};
