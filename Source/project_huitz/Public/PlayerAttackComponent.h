// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

UCLASS(Blueprintable)
class UWeapon : public UObject {
	GENERATED_BODY()
	
public:
	/** Name that should be shown for this weapon in-game (e.g. in UI, or as you go to pick it up from the ground) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PrettyName;
	
	/** Amount of HP this weapon's attacks should deal in damage, before any extra modifiers (headshot, weakness, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float DamagePerHit = 0.0f;
	
	/** Maximum range of this weapon's attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float Range = 25000.0f; // 250m
	
	/** How long to wait before allowing another attack, starting from when the previous attack's animation ends */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
	float AttackCooldown = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* StaticMesh;
};

UCLASS()
class UMeleeWeapon : public UWeapon {
	GENERATED_BODY()
};

UCLASS()
class URangedWeapon : public UWeapon {
	GENERATED_BODY()
	
public:
	/** Whether the weapon's attacks should be able to deal critical damage to targets' weak spots */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanHeadshot = true;
	
	/** Whether the weapon's attacks should deal less damage against distant targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasDamageFalloff = true;
	
	/** The distance at which the damage falloff should start to be applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float DamageFalloffInitialThreshold = 5000.0f; // 50m
	
	/** The distance at which the damage falloff should be fully applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float DamageFalloffCompleteThreshold = 15000.0f; // 150m
	
	/** The 'worst-case' damage falloff multiplier, see DamageFalloff(Initial/Complete)Threshold for more info */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float DamageFalloffMultiplier = 0.5f;
};

UCLASS()
class UBow : public URangedWeapon {
	GENERATED_BODY()
	
public:
	/** How long it takes to draw the bow to max charge */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
	float TimeToMaxCharge = 1.0f;
};

UCLASS()
class UGun : public URangedWeapon {
	GENERATED_BODY()
	
public:
	/** How many shots this weapon can fire before needing to be reloaded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	int AmmoPerClip = 12;
	
	int AmmoRemainingInClip = AmmoPerClip;
	
	/** Whether this gun should continuously fire while the input is held (true), or only fire once (false) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAutomatic = false;
	
	/** How long the player must wait in order to reload this gun */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
	float TimeTakenToReload = 1.0f;
	
	float bIsReloading = false;
};

UCLASS(Blueprintable)
class PROJECT_HUITZ_API UPlayerAttackComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPlayerAttackComponent();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	TObjectPtr<UWeapon> ActiveWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	TArray<TSubclassOf<UWeapon>> OwnedWeapons;
	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	void GiveNewWeapon(TSubclassOf<UWeapon> NewWeapon, bool bShouldEquipWeapon);
	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	void EquipWeapon(int WeaponIndex);
	
	virtual void OnPrimaryAttackInputDown();
	virtual void OnPrimaryAttackInputReleased();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPrimaryAttackInputDown")
	void OnPrimaryAttackInputDown_Event();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPrimaryAttackInputReleased")
	void OnPrimaryAttackInputReleased_Event();
	
	virtual void PerformAttack();
	
	virtual void PerformReload();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPerformAttack();
	
	bool bIsPrimaryAttackInputHeld;
	
	float ActiveCooldown = 0.0f;
	
	APlayerCharacter* GetPlayerCharacter() const {
		return static_cast<APlayerCharacter*>(GetOwner());
	}
};
