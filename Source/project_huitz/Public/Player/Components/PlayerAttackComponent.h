// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Weapons.h"
#include "PlayerAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntValueChangedSignature, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveWeaponChangedSignature, bool, bDoesWeaponUseAmmo);

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
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
	
	virtual void OnReloadInputDown();
	
	FOnIntValueChangedSignature OnCurrentAmmoChangedDelegate;
	FOnIntValueChangedSignature OnMaxAmmoChangedDelegate;
	FOnActiveWeaponChangedSignature OnActiveWeaponChangedDelegate;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugAttackTraces;
	
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
	float bIsReloading = false;
};
