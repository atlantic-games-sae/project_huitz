// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons.generated.h"

UCLASS(Abstract)
class UWeapon : public UObject {
	GENERATED_BODY()
	
public:
	UWeapon();
	
	/** Name that should be shown for this weapon in-game (e.g. in UI, or as you go to pick it up from the ground) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PrettyName;
	
	/** Amount of HP this weapon's attacks should deal in damage, before any extra modifiers (headshot, weakness, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float DamagePerHit;
	
	/** Maximum range of this weapon's attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float Range;
	
	/** How long to wait before allowing another attack, starting from when the previous attack's animation ends */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
	float AttackCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh;
};

UCLASS(Abstract, Blueprintable)
class UMeleeWeapon : public UWeapon {
	GENERATED_BODY()
	
public:
	UMeleeWeapon();
	
	/** The angle away from the crosshair that the cone uses to spread outwards */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Degrees"))
	float AttackConeHalfAngle;
};

UCLASS(Abstract)
class URangedWeapon : public UWeapon {
	GENERATED_BODY()
	
public:
	URangedWeapon();
	
	// /** Whether the weapon's attacks should be able to deal critical damage to targets' weak spots */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// bool bCanHeadshot = true;
	//
	// /** What the damage dealt by a headshot should be multiplied by, compared to a regular attack */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1))
	// float HeadshotMultiplier = 2.0f;
	
	/** Whether the weapon's attacks should deal less damage against distant targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasDamageFalloff;
	
	/** The distance at which the damage falloff should start to be applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float DamageFalloffInitialThreshold;
	
	/** The distance at which the damage falloff should be fully applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Centimeters"))
	float DamageFalloffCompleteThreshold;
	
	/** The 'worst-case' damage falloff multiplier, see DamageFalloff(Initial/Complete)Threshold for more info */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, ClampMax=1, UIMax=1))
	float DamageFalloffMultiplier;
};

// UCLASS(Abstract, Blueprintable)
// class UBow : public URangedWeapon {
// 	GENERATED_BODY()
// 	
// public:
// 	/** How long it takes to draw the bow to max charge */
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
// 	float TimeToMaxCharge = 1.0f;
// };

UCLASS(Abstract, Blueprintable)
class UGun : public URangedWeapon {
	GENERATED_BODY()
	
public:
	UGun();
	
	/** How many shots this weapon can fire before needing to be reloaded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	int AmmoPerClip ;
	
	UPROPERTY()
	int AmmoRemainingInClip;
	
	/** Whether this gun should continuously fire while the input is held (true), or only fire once (false) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAutomatic;
	
	/** How long the player must wait in order to reload this gun */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0, Units="Seconds"))
	float TimeTakenToReload;
};
