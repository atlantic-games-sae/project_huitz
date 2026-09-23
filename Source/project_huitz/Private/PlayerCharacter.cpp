// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CustomMovementComponent.h"
#include "HelperFunctions.h"

// Sets default values
APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	CharacterHeight = 190.0f;
	CapsuleRadiusProportionalToHeight = 0.15f;

	CrouchingHeightPercentage = 0.6f;
    SlidingHeightPercentage = 0.35f;
    HeightTransitionSpeed = 750.0f;

	MaxHealth = 50;
	CurrentHealth = 0;

	CurrentMovementInput = FVector2D().ZeroVector;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = GetCapsuleComponent();
	CapsuleComponent->SetCapsuleHalfHeight(CharacterHeight / 2.0f);
	CapsuleComponent->SetCapsuleRadius(CharacterHeight * CapsuleRadiusProportionalToHeight);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	MovementComponent = static_cast<UCustomMovementComponent*>(GetCharacterMovement());

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CapsuleComponent);
	Camera->SetRelativeLocation(FVector(0,0,CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.0));
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay() {
	OnMaxHealthChangedDelegate.Broadcast(MaxHealth);

	CapsuleComponent->SetCapsuleHalfHeight(CharacterHeight / 2.0f);
	CapsuleComponent->SetCapsuleRadius(CharacterHeight / 5.65f);

	CurrentHealth = MaxHealth;
	OnCurrentHealthChangedDelegate.Broadcast(CurrentHealth);

	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value) {
	CurrentMovementInput = Value.Get<FVector2D>();

	if (Controller == nullptr) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, CurrentMovementInput.Y, false);
	AddMovementInput(RightDirection, CurrentMovementInput.X, false);
}

void APlayerCharacter::Look(const FInputActionValue& Value) {
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::Jump() {
	EMovementState allowedStates[] = { None, Walking, Sliding, Dashing };
	bool shouldJump = false;

	EMovementState MovementState = MovementComponent->GetMovementState();

	for (int i = 0; i < sizeof(allowedStates); i++) {
		if (MovementState == allowedStates[i]) shouldJump = true;
	}

	if (!shouldJump) MovementComponent->TryWallJump(CapsuleComponent->GetScaledCapsuleHalfHeight(), CapsuleComponent->GetScaledCapsuleRadius());
	else {
		Super::Jump();
		MovementComponent->SetMovementState(EMovementState::Falling);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::ResetMovementInput);

		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::Dash);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::priv_Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::priv_UnCrouch);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	AdjustHeight(DeltaTime);

	Super::Tick(DeltaTime);
}

void APlayerCharacter::AdjustHeight(float DeltaTime) {
	EMovementState MovementState = MovementComponent->GetMovementState();
	if (MovementState != Crouching && MovementState != Sliding && !HasRoomToStand(HeightTransitionSpeed * DeltaTime)) return;
	float TargetHeight = CharacterHeight;
	if (MovementState == Crouching) TargetHeight *= CrouchingHeightPercentage;
	else if (MovementState == Sliding) TargetHeight *= SlidingHeightPercentage;
	CapsuleComponent->SetCapsuleHalfHeight(UHelperFunctions::FloatMoveTowards(CapsuleComponent->GetScaledCapsuleHalfHeight(), TargetHeight / 2.0f, (HeightTransitionSpeed / 2.0f) * DeltaTime));
}

bool APlayerCharacter::HasRoomToStand(float IntendedHeightDelta) const {
	FVector Start = GetActorLocation() + FVector(0,0,CapsuleComponent->GetScaledCapsuleHalfHeight());
	FVector End = Start + FVector(0,0,IntendedHeightDelta);
	FHitResult OutHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this, Start, End, CapsuleComponent->GetScaledCapsuleRadius(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, {}, EDrawDebugTrace::None, OutHit, true);
	return !bHit;
}

void APlayerCharacter::Crouch(bool bClientSimulation) {
    MovementComponent->SetDesiredCrouchState(true);
}

void APlayerCharacter::UnCrouch(bool bClientSimulation) {
    MovementComponent->SetDesiredCrouchState(false);
}

void APlayerCharacter::Dash() {
	MovementComponent->Dash(CurrentMovementInput);
}
