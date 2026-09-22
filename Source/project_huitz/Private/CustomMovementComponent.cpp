#include "CustomMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HelperFunctions.h"

UCustomMovementComponent::UCustomMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    MovementState = Falling;
    DesiredSprintState = false;
    DesiredCrouchState = false;

    // Initialising variables

    WalkingSpeed = 475.0f;
    SprintingSpeed = 750.0f;
    CrouchingSpeed = 200.0f;
    SlideBoost = 100.0f;

    DesiredMaxWalkSpeed = &WalkingSpeed;

    WalkingAcceleration = 1100.0f;
    SprintingAcceleration = 1300.0f;
    CrouchingAcceleration = 850.0f;

    BrakingDeceleration = 4000.0f;
    SlidingDeceleration = 300.0f;
    MinGroundDeceleration = 1000.0f;
    MaxGroundDeceleration = 1600.0f;

    WallJumpAllowedRange = 15.0f;
    WallJumpHorizontalKickStrength = 400.0f;
    WallJumpPercentageOfJumpVelocity = 0.9f;

    // Any variables inherited from CharacterMovementComponent that have their defaults overriden
    MaxWalkSpeed = WalkingSpeed;
    AirControl = 1.0f;
    bUseSeparateBrakingFriction = true;
    BrakingDecelerationWalking = BrakingDeceleration;
    BrakingDecelerationFalling = 300.0f;
    bUseFlatBaseForFloorChecks = true;
    SetWalkableFloorAngle(45.0f);
}

void UCustomMovementComponent::BeginPlay() {
    Super::BeginPlay();

    SetMovementState(Walking);
}

void UCustomMovementComponent::SetMovementState(EMovementState NewState) {
    if (NewState == MovementState) return;
    switch(NewState) {
        case Walking:
            DesiredMaxWalkSpeed = &WalkingSpeed;
            MaxAcceleration = WalkingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            break;
        case Sprinting:
            DesiredMaxWalkSpeed = &SprintingSpeed;
            MaxAcceleration = SprintingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            break;
        case Crouching:
            DesiredMaxWalkSpeed = &CrouchingSpeed;
            MaxAcceleration = CrouchingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            break;
        case Sliding:
            MaxWalkSpeed = FMath::Clamp(MaxWalkSpeed + SlideBoost, 0.0f, SprintingSpeed + SlideBoost);
            DesiredMaxWalkSpeed = new float(0.0f);
            MaxAcceleration = 0.0f;
            BrakingDecelerationWalking = 0.0f;
            SlideDirection = GetHorizontalVelocity().GetSafeNormal();
            Velocity = FVector(SlideDirection.X, SlideDirection.Y, Velocity.Z) * FMath::Clamp(GetHorizontalVelocity().Length() + SlideBoost, 0.0, SprintingSpeed + SlideBoost);
            break;
        case Falling:
            if (MovementState == Sliding) {
                MaxAcceleration = SprintingAcceleration;
            }
        default:
            break;
    }
    MovementState = NewState;
}

void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) {
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (MovementState == Falling && MovementMode == MOVE_Walking) {
        if (DesiredCrouchState) CrouchOrSlideBasedOnHorizontalVelocity();
        else if (GetHorizontalVelocity().Length() > 0.0) {
            if (DesiredSprintState) SetMovementState(Sprinting);
            else SetMovementState(Walking);
        } else SetMovementState(None);
    }

    if (MovementMode == MOVE_Falling && MovementState != Falling) {
        SetMovementState(Falling);
    }
}

void UCustomMovementComponent::SetDesiredSprintState(bool value) {
    DesiredSprintState = value;

    if (DesiredSprintState == (MovementState == Sprinting)) return;

    if (DesiredSprintState) SetMovementState(Sprinting);
    else {
        if (GetHorizontalVelocity().Length() > 0.0) SetMovementState(Walking);
        else SetMovementState(None);
    }
}

void UCustomMovementComponent::SetDesiredCrouchState(bool value) {
    DesiredCrouchState = value;
    if (DesiredCrouchState) {
        if (MovementState == None || MovementState == Walking) SetMovementState(Crouching);
        else if (MovementState == Sprinting) SetMovementState(Sliding);
    } else if (MovementState == Crouching || MovementState == Sliding) {
        if (GetHorizontalVelocity().Length() > 0.0) {
            if (DesiredSprintState) SetMovementState(Sprinting);
            else SetMovementState(Walking);
        } else SetMovementState(None);
    }
}

EMovementState UCustomMovementComponent::GetMovementState() const {
    return MovementState;
}

void UCustomMovementComponent::CrouchOrSlideBasedOnHorizontalVelocity() {
    if ((GetHorizontalVelocity().Length() - WalkingSpeed) > 0.1) SetMovementState(Sliding);
    else SetMovementState(Crouching);
}

FVector2D UCustomMovementComponent::GetHorizontalVelocity() const {
    return FVector2D(Velocity.X, Velocity.Y);
}

void UCustomMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    if (MovementState == None && GetHorizontalVelocity().Length() > 0.0) {
        if (DesiredSprintState) SetMovementState(Sprinting);
        else SetMovementState(Walking);
    } else if ((MovementState == Sprinting || MovementState == Walking) && GetHorizontalVelocity().Length() == 0.0) {
        SetMovementState(None);
    }

    UpdateMaxWalkSpeed(DeltaTime);

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateSlidingVelocity(DeltaTime);
}

void UCustomMovementComponent::UpdateMaxWalkSpeed(float DeltaTime) {
    if (MaxWalkSpeed == *DesiredMaxWalkSpeed) return;
    float MaxDelta = 0.0f;

    if (*DesiredMaxWalkSpeed > MaxWalkSpeed) {
        switch (MovementState) {
            case Walking:
                MaxDelta = WalkingAcceleration;
                break;
            case Sprinting:
                MaxDelta = SprintingAcceleration;
                break;
            case Crouching:
                MaxDelta = CrouchingAcceleration;
                break;
        }
    } else {
        if (MovementState == Sliding) MaxDelta = SlidingDeceleration;
        else if (MovementState != Falling) {
            MaxDelta = FMath::Lerp(MinGroundDeceleration, MaxGroundDeceleration, UHelperFunctions::GetAlphaInRange(GetHorizontalVelocity().Length(), CrouchingSpeed, SprintingSpeed));
        }
    }

    if (MaxDelta == 0.0f) return;

    MaxDelta *= DeltaTime;
    MaxWalkSpeed = UHelperFunctions::FloatMoveTowards(MaxWalkSpeed, *DesiredMaxWalkSpeed, MaxDelta);
}

void UCustomMovementComponent::UpdateSlidingVelocity(float DeltaTime) {
    if (MovementState != Sliding) return;

    if (GetHorizontalVelocity().Length() <= CrouchingSpeed) SetMovementState(Crouching);
    else {
        SlideVelocity = UHelperFunctions::FloatMoveTowards(GetHorizontalVelocity().Length(), 0.0f, SlidingDeceleration * DeltaTime);
        Velocity = (FVector(SlideDirection.X, SlideDirection.Y, 0.0) * SlideVelocity) + FVector(0.0, 0.0, Velocity.Z);
    }
}