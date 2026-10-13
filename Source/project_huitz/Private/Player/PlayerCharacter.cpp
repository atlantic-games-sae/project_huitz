// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Components/CustomMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HelperFunctions.h"
#include "Player/Components/PlayerAttackComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	CharacterHeight = 190.0f;
	CapsuleRadiusProportionalToHeight = 0.15f;

	CrouchingHeightPercentage = 0.6f;
    SlidingHeightPercentage = 0.35f;
    HeightTransitionSpeed = 750.0f;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	CurrentMovementInput = FVector2D().ZeroVector;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterHeight / 2.0f);
	GetCapsuleComponent()->SetCapsuleRadius(CharacterHeight * CapsuleRadiusProportionalToHeight);

	MovementComponent = static_cast<UCustomMovementComponent*>(GetCharacterMovement());

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0));
	Camera->bUsePawnControlRotation = true;
	
	InteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(FName("InteractionComponent"));
	
	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(FName("AttackComponent"));
}

void APlayerCharacter::BeginPlay() {
	GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterHeight / 2.0f);
	GetCapsuleComponent()->SetCapsuleRadius(CharacterHeight * CapsuleRadiusProportionalToHeight);
	
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), FoundActors);
	FoundActors.Remove(this);
	for (int i = 0; i < FoundActors.Num(); i++) {
		APlayerCharacter* TargetActor = Cast<APlayerCharacter>(FoundActors[i]);
		TargetActor->GetCapsuleComponent()->MoveIgnoreActors.AddUnique(this);
	}
	GetCapsuleComponent()->MoveIgnoreActors = FoundActors;
}

void APlayerCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(NewController)) {
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		UPlayerHUD* InstancedHUDWidget = CreateWidget<UPlayerHUD, APlayerController*>(PlayerController, PlayerHUDWidgetClass);
		InstancedHUDWidget->OnCurrentHealthChanged(CurrentHealth);
		InstancedHUDWidget->OnMaxHealthChanged(MaxHealth);
		
		OnMaxHealthChangedDelegate.AddUniqueDynamic(InstancedHUDWidget, &UPlayerHUD::OnMaxHealthChanged);
		OnCurrentHealthChangedDelegate.AddUniqueDynamic(InstancedHUDWidget, &UPlayerHUD::OnCurrentHealthChanged);
		
		if (UGun* AsGun = Cast<UGun>(AttackComponent->ActiveWeapon)) {
			InstancedHUDWidget->OnMaxAmmoChanged(AsGun->AmmoPerClip);
			InstancedHUDWidget->OnCurrentAmmoChanged(AsGun->AmmoRemainingInClip);
			InstancedHUDWidget->OnActiveWeaponChanged(true);
		}
		
		AttackComponent->OnMaxAmmoChangedDelegate.AddUniqueDynamic(InstancedHUDWidget, &UPlayerHUD::OnMaxAmmoChanged);
		AttackComponent->OnCurrentAmmoChangedDelegate.AddUniqueDynamic(InstancedHUDWidget, &UPlayerHUD::OnCurrentAmmoChanged);
		AttackComponent->OnActiveWeaponChangedDelegate.AddUniqueDynamic(InstancedHUDWidget, &UPlayerHUD::OnActiveWeaponChanged);
		
		InstancedHUDWidget->AddToPlayerScreen();
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
	EMovementState AllowedStates[] = { EMovementState::None, EMovementState::Walking, EMovementState::Sliding, EMovementState::Dashing };
	bool bCanJump = false;

	EMovementState MovementState = MovementComponent->GetMovementState();

	for (int i = 0; i < sizeof(AllowedStates); i++) {
		if (MovementState == AllowedStates[i]) bCanJump = true;
	}

	if (bCanJump) {
		Super::Jump();
		MovementComponent->SetMovementState(EMovementState::Falling);
	} else if (MovementState == EMovementState::Falling) {
		if (!MovementComponent->TryWallJump(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius()) && JumpCurrentCount < JumpMaxCount) {
			Super::Jump();
			MovementComponent->SetMovementState(EMovementState::Falling);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::ClearMovementInput);

		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::Dash);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::priv_Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::priv_UnCrouch);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent, &UPlayerInteractionComponent::OnInteractInputDown);
	
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, AttackComponent, &UPlayerAttackComponent::OnPrimaryAttackInputDown);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Completed, AttackComponent, &UPlayerAttackComponent::OnPrimaryAttackInputReleased);
		
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, AttackComponent, &UPlayerAttackComponent::OnReloadInputDown);
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
	if (MovementState != EMovementState::Crouching && MovementState != EMovementState::Sliding && !HasRoomToStand(HeightTransitionSpeed * DeltaTime)) return;
	float TargetHeight = CharacterHeight;
	if (MovementState == EMovementState::Crouching) TargetHeight *= CrouchingHeightPercentage;
	else if (MovementState == EMovementState::Sliding) TargetHeight *= SlidingHeightPercentage;
	GetCapsuleComponent()->SetCapsuleHalfHeight(UHelperFunctions::FloatMoveTowards(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), TargetHeight / 2.0f, (HeightTransitionSpeed / 2.0f) * DeltaTime));
}

bool APlayerCharacter::HasRoomToStand(float IntendedHeightDelta) const {
	FVector Start = GetActorLocation() + FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector End = Start + FVector(0,0,IntendedHeightDelta);
	FHitResult OutHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this, Start, End, GetCapsuleComponent()->GetScaledCapsuleRadius(),
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
