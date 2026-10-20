#include "Player/Components/CustomMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HelperFunctions.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Player/PlayerCharacter.h"

UCustomMovementComponent::UCustomMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    CustomMovementState = EMovementState::Falling;
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, CustomMovementState, this);
    DesiredSlideState = false;

    // Initialising variables

    WalkingSpeed = 600.0f;
    SlideBoost = 500.0f;
    
    bSlideBoostChangesDirection = true;

    DashDuration = 0.2f;
    DashVelocity = 1050.0f;

    DesiredMaxWalkSpeed = WalkingSpeed;
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, DesiredMaxWalkSpeed, this);

    WalkingAcceleration = 1250.0f;

    BrakingDeceleration = 4000.0f;
    SlidingDeceleration = 450.0f;
    MinAirDeceleration = 50.0f;
    MaxAirDeceleration = 850.0f;
    MinGroundDeceleration = 500.0f;
    MaxGroundDeceleration = 2250.0f;

    WallJumpAllowedRange = 16.0f;
    WallJumpHorizontalKickStrength = 275.0f;
    WallJumpPercentageOfJumpVelocity = 0.875f;

    EnableAirDeceleration = true;
    HasSlideBoosted = false;
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, HasSlideBoosted, this);
    
    TotalStaminaPoints = 3;
    StaminaRegenPerSecond = 0.5f;
    
    // Any variables inherited from CharacterMovementComponent that have their defaults overriden
    SetIsReplicatedByDefault(true);
    MaxWalkSpeed = WalkingSpeed;
    AirControl = 0.9f;
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
    
    CurrentStamina = TotalStaminaPoints;

    SetMovementState(EMovementState::Walking);
}

void UCustomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    if (CustomMovementState == EMovementState::Dashing) ActiveDashTimer -= DeltaTime;
    
    if (CurrentStamina < TotalStaminaPoints) {
        if (TotalStaminaPoints - CurrentStamina < StaminaRegenPerSecond * DeltaTime) {
            CurrentStamina = TotalStaminaPoints;
        } else CurrentStamina += StaminaRegenPerSecond * DeltaTime;
    }
    
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomMovementComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    FDoRepLifetimeParams SharedParams;
    SharedParams.bIsPushBased = true;
    
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, CustomMovementState, SharedParams);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, ActiveDashDirection, SharedParams);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, DesiredMaxWalkSpeed, SharedParams);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, DesiredSlideState, SharedParams);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, SlideDirection, SharedParams);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCustomMovementComponent, HasSlideBoosted, SharedParams);
}

void UCustomMovementComponent::SetMovementState_Implementation(EMovementState NewState) {
    if (NewState == CustomMovementState) return;
    switch(NewState) {
        case EMovementState::None:
            DesiredMaxWalkSpeed = WalkingSpeed;
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, DesiredMaxWalkSpeed, this);
            MaxAcceleration = WalkingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            HasSlideBoosted = false;
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, HasSlideBoosted, this);
            break;
        case EMovementState::Walking:
            DesiredMaxWalkSpeed = WalkingSpeed;
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, DesiredMaxWalkSpeed, this);
            MaxAcceleration = WalkingAcceleration;
            BrakingDecelerationWalking = BrakingDeceleration;
            HasSlideBoosted = false;
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, HasSlideBoosted, this);
            break;
        case EMovementState::Sliding:
            DesiredMaxWalkSpeed = 0.0f;
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, DesiredMaxWalkSpeed, this);
            MaxAcceleration = 0.0f;
            BrakingDecelerationWalking = 0.0f;
            
            {
                TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
                if (IsValid(PlayerCharacter)) {
                    FVector2D CurrentMovementInput = PlayerCharacter->GetCurrentMovementInput();
                    if (CurrentMovementInput != FVector2D::ZeroVector && !HasSlideBoosted && bSlideBoostChangesDirection) {
                        SlideDirection = UHelperFunctions::MovementInputToWorldSpace(CurrentMovementInput, PlayerCharacter->GetActorRotation().Yaw);
                    } else {
                        SlideDirection = GetHorizontalVelocity().GetSafeNormal();
                    }
                }
            }
            MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, SlideDirection, this);
            
            if (!HasSlideBoosted) {
                MaxWalkSpeed = FMath::Clamp(MaxWalkSpeed + SlideBoost, 0.0f, DashVelocity);
                Velocity = FVector(SlideDirection.X, SlideDirection.Y, 0) * FMath::Clamp(GetHorizontalVelocity().Length() + SlideBoost, 0.0, DashVelocity);
                HasSlideBoosted = true;
                MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, HasSlideBoosted, this);
            }
        
            break;
        case EMovementState::Falling:
            if (CustomMovementState == EMovementState::Sliding) {
                MaxAcceleration = WalkingAcceleration;
            }
            break;
        case EMovementState::Dashing:
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
    if (NewState != EMovementState::Dashing) {
        GravityScale = 1.0f;
        if (CustomMovementState != EMovementState::Dashing) EnableAirDeceleration = true;
    }
    
    if (GetNetMode() == NM_DedicatedServer) UE_LOG(LogTemp, Display, TEXT("Changed state to %s"), *(UEnum::GetValueAsString(NewState)));
    //if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Changed state to %s"), *(UEnum::GetValueAsString(NewState))));
    
    CustomMovementState = NewState;
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, CustomMovementState, this);
}

void UCustomMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) {
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    if (CustomMovementState == EMovementState::Falling && MovementMode == MOVE_Walking) {
        if (DesiredSlideState) SetMovementState_Server(EMovementState::Sliding);
        else if (GetHorizontalVelocity().Length() > 0.0) SetMovementState(EMovementState::Walking);
        else SetMovementState(EMovementState::None);
    } else if (CustomMovementState != EMovementState::Falling && MovementMode == MOVE_Falling) {
        SetMovementState(EMovementState::Falling);
    }
}

void UCustomMovementComponent::SetDesiredSlideState(bool value) {
    DesiredSlideState = value;
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, DesiredSlideState, this);
    
    OnDesiredSlideStateChanged();
    
    if (GetOwnerRole() == ROLE_AutonomousProxy) SetDesiredSlideState_Server(value);
}

void UCustomMovementComponent::OnDesiredSlideStateChanged() {
    if (DesiredSlideState) {
        if (CustomMovementState == EMovementState::Walking) SetMovementState(EMovementState::Sliding);
    } else if (CustomMovementState == EMovementState::Sliding) {
        if (GetHorizontalVelocity().Length() > 0.0) SetMovementState(EMovementState::Walking);
        else SetMovementState(EMovementState::None);
    }
}

bool UCustomMovementComponent::TryWallJump(float CapsuleHalfHeight, float CapsuleRadius) {
    if (CustomMovementState != EMovementState::Falling) return false;

    FVector TracePoint = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - CapsuleHalfHeight / 2.0f);
    float Radius = CapsuleRadius + WallJumpAllowedRange;
    FHitResult OutHit;
    bool bHit = UKismetSystemLibrary::SphereTraceSingle(
        this, TracePoint, TracePoint, Radius,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false, {}, EDrawDebugTrace::None, OutHit, true);
    if (!bHit) return false;

    FVector DistanceFromWallHit = GetOwner()->GetActorLocation() - OutHit.ImpactPoint;
    FVector2D KickAwayFromWall = FVector2D(DistanceFromWallHit.X, DistanceFromWallHit.Y).GetSafeNormal() * WallJumpHorizontalKickStrength;
    Velocity = FVector(Velocity.X + KickAwayFromWall.X,
        Velocity.Y + KickAwayFromWall.Y,
        JumpZVelocity * WallJumpPercentageOfJumpVelocity);
    if (GetOwnerRole() == ROLE_AutonomousProxy) SetVelocity_Server(Velocity);
    return true;
}

EMovementState UCustomMovementComponent::GetMovementState() const {
    return CustomMovementState;
}

FVector2D UCustomMovementComponent::GetHorizontalVelocity() const {
    return FVector2D(Velocity.X, Velocity.Y);
}

void UCustomMovementComponent::PerformMovement(float DeltaTime) {
    if (CustomMovementState == EMovementState::None && GetHorizontalVelocity().Length() > 0.0) {
        if (DesiredSlideState) SetMovementState(EMovementState::Sliding);
        else SetMovementState(EMovementState::Walking);
    } else if (CustomMovementState == EMovementState::Walking && GetHorizontalVelocity().Length() == 0.0) {
        SetMovementState(EMovementState::None);
    }

    UpdateMaxWalkSpeed(DeltaTime);

    float PreTickHorizontalVelocityLength = FVector2D(Velocity).Length();

    Super::PerformMovement(DeltaTime);
    
    switch (CustomMovementState) {
        case EMovementState::Dashing:
            if (DesiredSlideState) SetMovementState(EMovementState::Sliding);
            else if (ActiveDashTimer <= 0) {
                if (MovementMode == MOVE_Falling) SetMovementState(EMovementState::Falling);
                else SetMovementState(EMovementState::Walking);
            } else {
                Velocity = FVector(ActiveDashDirection.X, ActiveDashDirection.Y, 0).GetSafeNormal() * DashVelocity;
            }
            break;
        case EMovementState::Sliding:
            UpdateSlidingVelocity(DeltaTime);
            break;
        case EMovementState::Falling:
            if (EnableAirDeceleration && PreTickHorizontalVelocityLength > WalkingSpeed) {
                float AirDeceleration = FMath::Lerp(MinAirDeceleration, MaxAirDeceleration, FMath::Square(UHelperFunctions::GetAlphaInRange(PreTickHorizontalVelocityLength, WalkingSpeed, DashVelocity)));
                Velocity = FVector(Velocity.X, Velocity.Y, 0).GetSafeNormal() * UHelperFunctions::FloatMoveTowards(PreTickHorizontalVelocityLength, 0, AirDeceleration * DeltaTime) + FVector(0, 0, Velocity.Z);
            }
            break;
        default:
            break;
    }
}

void UCustomMovementComponent::UpdateMaxWalkSpeed(float DeltaTime) {
    if (MaxWalkSpeed == DesiredMaxWalkSpeed) return;
    float MaxDelta = 0.0f;

    if (DesiredMaxWalkSpeed > MaxWalkSpeed) { // Accelerating
        if (CustomMovementState == EMovementState::Walking || CustomMovementState == EMovementState::None) MaxDelta = WalkingAcceleration;
    } else { // Decelerating
        if (CustomMovementState == EMovementState::Sliding) MaxDelta = SlidingDeceleration;
        else if (CustomMovementState != EMovementState::Falling) {
            MaxDelta = FMath::Lerp(MinGroundDeceleration, MaxGroundDeceleration, FMath::Square(UHelperFunctions::GetAlphaInRange(GetHorizontalVelocity().Length(), 0, DashVelocity)));
        }
    }

    if (MaxDelta == 0.0f) return;

    MaxDelta *= DeltaTime;
    MaxWalkSpeed = UHelperFunctions::FloatMoveTowards(MaxWalkSpeed, DesiredMaxWalkSpeed, MaxDelta);
}

void UCustomMovementComponent::UpdateSlidingVelocity(float DeltaTime) {
    if (CustomMovementState != EMovementState::Sliding) return;
    
    SlideVelocity = UHelperFunctions::FloatMoveTowards(GetHorizontalVelocity().Length(), 0, SlidingDeceleration * DeltaTime);
    Velocity = (FVector(SlideDirection.X, SlideDirection.Y, 0) * SlideVelocity) + FVector(0, 0, Velocity.Z);
}

void UCustomMovementComponent::Dash(FVector2D InputDirection) {
    if (InputDirection == FVector2D().ZeroVector) return;

    if (CustomMovementState == EMovementState::Sliding) return;
    
    if (CurrentStamina < 1.0f) return;
    
    CurrentStamina -= 1.0f;

    PerformDash(InputDirection);
    
    if (GetOwnerRole() == ROLE_AutonomousProxy) PerformDash_Server(InputDirection);
}

void UCustomMovementComponent::PerformDash(FVector2D InputDirection) {
    ActiveDashDirection = UHelperFunctions::MovementInputToWorldSpace(InputDirection, GetOwner()->GetActorRotation().Yaw);
    MARK_PROPERTY_DIRTY_FROM_NAME(UCustomMovementComponent, ActiveDashDirection, this);
    
    SetMovementState(EMovementState::Dashing);
}