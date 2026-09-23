#include "CustomMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HelperFunctions.h"

UCustomMovementComponent::UCustomMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    MovementState = Falling;
    DesiredCrouchState = false;

    // Initialising variables

    WalkingSpeed = 550.0f;
    CrouchingSpeed = 250.0f;
    SlideBoost = 450.0f;

    SlidingThreshold = 500.0f;

    DashDuration = 0.2f;
    DashVelocity = 1000.0f;

    DesiredMaxWalkSpeed = &WalkingSpeed;

    WalkingAcceleration = 1100.0f;
    CrouchingAcceleration = 850.0f;

    BrakingDeceleration = 4000.0f;
    SlidingDeceleration = 350.0f;
    MinAirDeceleration = 10.0f;
    MaxAirDeceleration = 1000.0f;
    MinGroundDeceleration = 500.0f;
    MaxGroundDeceleration = 2250.0f;

    WallJumpAllowedRange = 15.0f;
    WallJumpHorizontalKickStrength = 400.0f;
    WallJumpPercentageOfJumpVelocity = 0.9f;

    EnableAirDeceleration = true;
    HasSlideBoosted = false;

    // Any variables inherited from CharacterMovementComponent that have their defaults overriden
    MaxWalkSpeed = WalkingSpeed;
    AirControl = 1.0f;
    bUseSeparateBrakingFriction = true;
    BrakingDecelerationWalking = BrakingDeceleration;
    BrakingDecelerationFalling = 0.0f;
    FallingLateralFriction = 0.0f;
    BrakingFriction = 0.0f;
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
            HasSlideBoosted = false;
            break;
        case Crouching:
            DesiredMaxWalkSpeed = &CrouchingSpeed;
            MaxAcceleration = CrouchingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            HasSlideBoosted = false;
            break;
        case Sliding:
            DesiredMaxWalkSpeed = new float(0.0f);
            MaxAcceleration = 0.0f;
            BrakingDecelerationWalking = 0.0f;
            SlideDirection = GetHorizontalVelocity().GetSafeNormal();
            if (!HasSlideBoosted) {
                MaxWalkSpeed = FMath::Clamp(MaxWalkSpeed + SlideBoost, 0.0f, DashVelocity);
                Velocity = FVector(SlideDirection.X, SlideDirection.Y, 0) * FMath::Clamp(GetHorizontalVelocity().Length() + SlideBoost, 0.0, DashVelocity);
                HasSlideBoosted = true;
            }
            break;
        case Falling:
            if (MovementState == Sliding) {
                MaxAcceleration = WalkingAcceleration;
            }
            break;
        case Dashing:
            MaxWalkSpeed = DashVelocity;
            ActiveDashTimer = DashDuration;
            Velocity = FVector(ActiveDashDirection.X, ActiveDashDirection.Y, 0).GetSafeNormal() * DashVelocity;
            BrakingDecelerationWalking = 0.0f;
            GravityScale = 0.0f;
            EnableAirDeceleration = false;
            break;
        default:
            break;
    }
    if (NewState != Dashing) {
        GravityScale = 1.0f;
        if (MovementState != Dashing) EnableAirDeceleration = true;
    }
    MovementState = NewState;
}

void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) {
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (MovementState == Falling && MovementMode == MOVE_Walking) {
        if (DesiredCrouchState) CrouchOrSlideBasedOnHorizontalVelocity();
        else if (GetHorizontalVelocity().Length() > 0.0) {
            SetMovementState(Walking);
        } else SetMovementState(None);
    }

    if (MovementMode == MOVE_Falling && MovementState != Falling) {
        SetMovementState(Falling);
    }
}

void UCustomMovementComponent::SetDesiredCrouchState(bool value) {
    DesiredCrouchState = value;
    if (DesiredCrouchState) {
        if (MovementState == None) SetMovementState(Crouching);
        else if (MovementState == Walking) CrouchOrSlideBasedOnHorizontalVelocity();
    } else if (MovementState == Crouching || MovementState == Sliding) {
        if (GetHorizontalVelocity().Length() > 0.0) {
            SetMovementState(Walking);
        } else SetMovementState(None);
    }
}

void UCustomMovementComponent::TryWallJump(float CapsuleHalfHeight, float CapsuleRadius) {
    if (MovementState != EMovementState::Falling) return;

    FVector TracePoint = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - CapsuleHalfHeight / 2.0f);
    float Radius = CapsuleRadius + WallJumpAllowedRange;
    FHitResult OutHit;
    bool bHit = UKismetSystemLibrary::SphereTraceSingle(
        this, TracePoint, TracePoint, Radius,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false, {}, EDrawDebugTrace::None, OutHit, true);
    if (!bHit) return;

    FVector DistanceFromWallHit = GetOwner()->GetActorLocation() - OutHit.ImpactPoint;
    FVector2D KickAwayFromWall = FVector2D(DistanceFromWallHit.X, DistanceFromWallHit.Y).GetSafeNormal() * WallJumpHorizontalKickStrength;
    Velocity = FVector(Velocity.X + KickAwayFromWall.X,
        Velocity.Y + KickAwayFromWall.Y,
        JumpZVelocity * WallJumpPercentageOfJumpVelocity);
}

EMovementState UCustomMovementComponent::GetMovementState() const {
    return MovementState;
}

void UCustomMovementComponent::CrouchOrSlideBasedOnHorizontalVelocity() {
    if (GetHorizontalVelocity().Length() >= SlidingThreshold) SetMovementState(Sliding);
    else SetMovementState(Crouching);
}

FVector2D UCustomMovementComponent::GetHorizontalVelocity() const {
    return FVector2D(Velocity.X, Velocity.Y);
}

void UCustomMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    if (MovementState == None && GetHorizontalVelocity().Length() > 0.0) {
        SetMovementState(Walking);
    } else if (MovementState == Walking && GetHorizontalVelocity().Length() == 0.0) {
        SetMovementState(None);
    }

    if (MovementState == Dashing) {
        ActiveDashTimer -= DeltaTime;
        if (DesiredCrouchState) SetMovementState(Sliding);
        else if (ActiveDashTimer <= 0) {
            if (MovementMode == MOVE_Falling) SetMovementState(Falling);
            else SetMovementState(Walking);
        }
    } else UpdateMaxWalkSpeed(DeltaTime);

    float PreTickHorizontalVelocityLength = FVector2D(Velocity).Length();

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateSlidingVelocity(DeltaTime);

    if (EnableAirDeceleration && MovementState == Falling) {
        float AirDeceleration = FMath::Lerp(MinAirDeceleration, MaxAirDeceleration, FMath::Square(UHelperFunctions::GetAlphaInRange(PreTickHorizontalVelocityLength, WalkingSpeed, DashVelocity)));
        Velocity = FVector(Velocity.X, Velocity.Y, 0).GetSafeNormal() * UHelperFunctions::FloatMoveTowards(PreTickHorizontalVelocityLength, 0, AirDeceleration * DeltaTime) + FVector(0, 0, Velocity.Z);
    }
}

void UCustomMovementComponent::UpdateMaxWalkSpeed(float DeltaTime) {
    if (MaxWalkSpeed == *DesiredMaxWalkSpeed) return;
    float MaxDelta = 0.0f;

    if (*DesiredMaxWalkSpeed > MaxWalkSpeed) {
        switch (MovementState) {
            case Walking:
                MaxDelta = WalkingAcceleration;
                break;
            case Crouching:
                MaxDelta = CrouchingAcceleration;
                break;
        }
    } else {
        if (MovementState == Sliding) MaxDelta = SlidingDeceleration;
        else if (MovementState != Falling) {
            MaxDelta = FMath::Lerp(MinGroundDeceleration, MaxGroundDeceleration, FMath::Square(UHelperFunctions::GetAlphaInRange(GetHorizontalVelocity().Length(), 0, DashVelocity)));
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
        SlideVelocity = UHelperFunctions::FloatMoveTowards(GetHorizontalVelocity().Length(), 0, SlidingDeceleration * DeltaTime);
        Velocity = (FVector(SlideDirection.X, SlideDirection.Y, 0) * SlideVelocity) + FVector(0, 0, Velocity.Z);
    }
}

void UCustomMovementComponent::Dash(FVector2D InputDirection) {
    if (InputDirection == FVector2D().ZeroVector) return;

    switch (MovementState) {
        case Sliding:
            return;
        case Crouching:
            return;
    }

    ActiveDashDirection = FVector2D(InputDirection.X, -InputDirection.Y).GetRotated(GetOwner()->GetActorRotation().Yaw + 90.0);

    SetMovementState(Dashing);
}
