#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum EMovementState : int {
    /** Standing on a surface with no velocity, not crouched. */
	None,

	/** Walking on a surface. */
	Walking,

	/** Walking, but faster. */
	Sprinting,

	/** Walking, but slower + lower. */
	Crouching,

	/** Crouching, but more awesome. */
	Sliding,

	/** Falling under the effects of gravity , such as after walking off the edge of a surface, or after jumping. */
	Falling,

    /** Actively dashing in a horizontal direction. */
    Dashing
};

UCLASS()
class UCustomMovementComponent : public UCharacterMovementComponent {
    GENERATED_BODY()

    UCustomMovementComponent(const FObjectInitializer& ObjectInitializer);

public:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float WalkingSpeed;
	UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float SprintingSpeed;
	UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float CrouchingSpeed;
	UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float SlideBoost;

	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float WalkingAcceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float SprintingAcceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float CrouchingAcceleration;

	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float BrakingDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float SlidingDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float MinGroundDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float MaxGroundDeceleration;

	UPROPERTY(Category="Custom Movement|Wall Jumping", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float WallJumpAllowedRange;
	UPROPERTY(Category="Custom Movement|Wall Jumping", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float WallJumpHorizontalKickStrength;
	UPROPERTY(Category="Custom Movement|Wall Jumping", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, ClampMax = 1, UIMax=1))
	float WallJumpPercentageOfJumpVelocity;

    UFUNCTION(BlueprintPure, Category = "Custom Movement")
	EMovementState GetMovementState() const;

	UFUNCTION(BlueprintPure, Category = "Custom Movement")
	FVector2D GetHorizontalVelocity() const;

    void SetMovementState(EMovementState NewState);

    virtual void SetDesiredCrouchState(bool value);
    virtual void SetDesiredSprintState(bool value);

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

private:
	inline void CrouchOrSlideBasedOnHorizontalVelocity();

	void UpdateMaxWalkSpeed(float DeltaTime);

	void UpdateSlidingVelocity(float DeltaTime);

	EMovementState MovementState;

	bool DesiredSprintState;
	bool DesiredCrouchState;

	float* DesiredMaxWalkSpeed;
	
	FVector2D SlideDirection;
	float SlideVelocity;
};
