// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int, Health);

class UCustomMovementComponent;

UCLASS()
class PROJECT_HUITZ_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);

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
	UInputAction* PrimaryFireAction;

	UPROPERTY(Category="Input", EditAnywhere)
	UInputAction* ReloadAction;

	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="Centimeters"))
	float CharacterHeight;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ClampMax="0.5", UIMax="0.5"))
	float CapsuleRadiusProportionalToHeight;

	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ClampMax="1", UIMax="1"))
	float CrouchingHeightPercentage;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ClampMax="1", UIMax="1"))
	float SlidingHeightPercentage;
	UPROPERTY(Category="Height", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="CentimetersPerSecond"))
	float HeightTransitionSpeed;

	UPROPERTY(Category="General", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	int MaxHealth;

	UPROPERTY(Category = "Components", EditAnywhere)
	UCameraComponent* Camera = nullptr;

	virtual void Jump() override;

	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Dash();

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadOnly)
	UCustomMovementComponent* MovementComponent;

	FOnHealthChangedSignature OnCurrentHealthChangedDelegate;
	FOnHealthChangedSignature OnMaxHealthChangedDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AdjustHeight(float DeltaTime);
	bool HasRoomToStand(float IntendedHeightDelta) const;

	int CurrentHealth;

	UCapsuleComponent* CapsuleComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void priv_Crouch() { Crouch(); } // private alias for Crouch() that has a void() signature, rather than a void(bool) one, to allow binding it with the Enhanced Input System
	void priv_UnCrouch() { UnCrouch(); }

	FVector2D CurrentMovementInput;

	void ResetMovementInput() {
		CurrentMovementInput = FVector2D().ZeroVector;
	};
};
