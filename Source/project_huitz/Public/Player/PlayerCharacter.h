// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"
#include "Player/Components/PlayerInteractionComponent.h"
#include "Player/PlayerHUD.h"
#include "PlayerCharacter.generated.h"

struct FCharacterSaveData;
class ACustomPlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, int, Health);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDiedSignature);

class UCustomMovementComponent;
class UBaseAttackComponent;

UCLASS(Abstract)
class PROJECT_HUITZ_API APlayerCharacter : public ACustomCharacterBase {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	virtual void Jump() override;

	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Dash();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBaseAttackComponent* MyAttackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCustomMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPlayerInteractionComponent* MyInteractionComponent;

	FVector2D GetCurrentMovementInput() const {
		return CurrentMovementInput;
	}
	
	void RespawnAtLastCheckpoint();
	
	void RegisterCheckpoint(const FVector& CheckpointLocation, const float CheckpointYaw);
	
	bool GetIsDead() const { return bIsDead; }
	
	FOnPlayerDiedSignature OnPlayerDiedDelegate;
	
	void LoadPlayerSaveData(const FCharacterSaveData& SaveData);
	
	FTransform GetLastCheckpointTransform() const {
		FTransform ReturnValue = FTransform();
		ReturnValue.SetLocation(LastCheckpointLocation);
		ReturnValue.SetRotation(FRotator(0,LastCheckpointYaw,0).Quaternion());
		return ReturnValue;
	}

protected:
	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* MouseLookAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* LookAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* JumpAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* DashAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* SlideAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* PrimaryAttackAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* ReloadAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* InteractAction;

	UPROPERTY(Category="Input", EditDefaultsOnly)
	UInputAction* PauseGameAction;

	UPROPERTY(Category="Height", EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float CharacterHeight;
	UPROPERTY(Category="Height", EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=0.5, UIMax=0.5))
	float CapsuleRadiusProportionalToHeight;

	UPROPERTY(Category="Height", EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float SlidingHeightPercentage;
	UPROPERTY(Category="Height", EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float HeightTransitionSpeed;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AdjustHeight(float DeltaTime);
	bool HasRoomToStand(float IntendedHeightDelta) const;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly, DisplayName="Player HUD Widget Class")
	TSubclassOf<UPlayerHUD> PlayerHUDWidgetClass;

	virtual void OnDeath() override;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsDead;
	
	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DeathMessageWidgetClass;
	
private:
	// private aliases that have a void() signature, rather than void(bool), to allow binding with the Enhanced Input System
	void priv_Crouch() {
		Crouch();
	}
	void priv_UnCrouch() {
		UnCrouch();
	}

	FVector2D CurrentMovementInput;

	void ClearMovementInput() {
		CurrentMovementInput = FVector2D().ZeroVector;
	}

	ACustomPlayerController* GetCustomPlayerController() const;
	
	FVector LastCheckpointLocation;
	float LastCheckpointYaw;
};
