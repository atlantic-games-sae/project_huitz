#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EMovementState : uint8 {
    /** Standing on a surface with no velocity, not crouched. */
	None,

	/** Walking on a surface. */
	Walking,

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
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float WalkingSpeed;
	UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float CrouchingSpeed;
	UPROPERTY(Category="Custom Movement|Movement Speed", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float SlideBoost;

	UPROPERTY(Category="Custom Movement|Sliding", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float SlidingThreshold;

	UPROPERTY(Category="Custom Movement|Dashing", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1, Units="Seconds"))
	float DashDuration;
	UPROPERTY(Category="Custom Movement|Dashing", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecond"))
	float DashVelocity;

	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float WalkingAcceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float CrouchingAcceleration;

	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float BrakingDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float SlidingDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float MinAirDeceleration;
	UPROPERTY(Category="Custom Movement|Acceleration & Deceleration", EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, UIMin=0, Units="CentimetersPerSecondSquared"))
	float MaxAirDeceleration;
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

	UFUNCTION(NetMulticast, Reliable)
    void SetMovementState(EMovementState NewState);

    virtual void SetDesiredCrouchState(bool value);

	virtual bool TryWallJump(float CapsuleHalfHeight, float CapsuleRadius);

	virtual void Dash(FVector2D InputDirection);

protected:
	virtual void PerformMovement(float DeltaTime) override;
	
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Custom Movement|Acceleration & Deceleration")
	bool EnableAirDeceleration;

	UPROPERTY(Replicated)
	bool HasSlideBoosted;
	
	UPROPERTY(Replicated)
	EMovementState CustomMovementState;
	
	void PerformDash(FVector2D InputDirection);
	
	UFUNCTION(Server, Reliable)
	void SetVelocity_Server(FVector NewVelocity);
	void SetVelocity_Server_Implementation(FVector NewVelocity) { Velocity = NewVelocity; }
	
	UPROPERTY(Replicated)
	FVector2D ActiveDashDirection;

private:
	UFUNCTION(Server, Reliable)
	void PerformDash_Server(FVector2D InputDirection);
	void PerformDash_Server_Implementation(FVector2D InputDirection) { PerformDash(InputDirection); }
	
	UFUNCTION(Server, Reliable)
	void SetDesiredCrouchState_Server(bool value);
	void SetDesiredCrouchState_Server_Implementation(bool value) { SetDesiredCrouchState(value); }
	
	UFUNCTION(Server, Reliable)
	void CrouchOrSlideBasedOnHorizontalVelocity();

	void UpdateMaxWalkSpeed(float DeltaTime);

	void UpdateSlidingVelocity(float DeltaTime);

	UPROPERTY(ReplicatedUsing=OnRep_DesiredCrouchState)
	bool DesiredCrouchState;
	
	UFUNCTION()
	void OnRep_DesiredCrouchState() { OnDesiredCrouchStateChanged(); }
	
	void OnDesiredCrouchStateChanged();
	
	UPROPERTY(Replicated)
	float DesiredMaxWalkSpeed;
	
	UPROPERTY(Replicated)
	FVector2D SlideDirection;
	
	float SlideVelocity;

	float ActiveDashTimer;
};
