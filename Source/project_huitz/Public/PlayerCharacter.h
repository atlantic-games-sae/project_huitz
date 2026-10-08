// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterBase.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"

#include "PlayerCharacter.generated.h"

class UCustomMovementComponent;
class UPlayerAttackComponent;

UCLASS(Abstract)
class PROJECT_HUITZ_API APlayerCharacter : public ACustomCharacterBase {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(Category="Components", EditAnywhere)
	UCameraComponent* Camera;

	virtual void Jump() override;

	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Dash();

protected:
	UPROPERTY(Category="Input", EditAnywhere)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* MouseLookAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* LookAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* DashAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* CrouchAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* PrimaryAttackAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* ReloadAction;

	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float CharacterHeight;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=0.5, UIMax=0.5))
	float CapsuleRadiusProportionalToHeight;

	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float CrouchingHeightPercentage;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float SlidingHeightPercentage;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float HeightTransitionSpeed;
	
	UPROPERTY(Category="Components", EditAnywhere, BlueprintReadOnly)
	UCustomMovementComponent* MovementComponent;
	
	UPROPERTY(Category="Components", EditAnywhere, BlueprintReadOnly)
	UPlayerAttackComponent* AttackComponent;
	
	UPROPERTY(Category="UI", EditAnywhere, BlueprintReadOnly, DisplayName="Player HUD Widget Class")
	TSubclassOf<UPlayerHUD> PlayerHUDWidgetClass = UPlayerHUD::StaticClass();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AdjustHeight(float DeltaTime);
	bool HasRoomToStand(float IntendedHeightDelta) const;

private:
	void priv_Crouch() { Crouch(); } // private alias for Crouch() that has a void() signature, rather than a void(bool) one, to allow binding it with the Enhanced Input System
	void priv_UnCrouch() { UnCrouch(); }

	FVector2D CurrentMovementInput;

	void ClearMovementInput() {
		CurrentMovementInput = FVector2D().ZeroVector;
	}
};
