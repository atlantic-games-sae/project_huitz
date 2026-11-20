// © 2026 Atlantic Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Libraries/DelegateLibrary.h"
#include "Player/Weapons.h"
#include "BaseAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveWeaponChangedSignature, bool, bNewShouldDisplayAmmo);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_HUITZ_API UBaseAttackComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UBaseAttackComponent();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	void EquipWeapon(int WeaponIndex);
	
	virtual void OnPrimaryAttackInputDown();
	virtual void OnPrimaryAttackInputReleased();
	
	virtual void OnReloadInputDown();
	
	FOnIntValueChangedSignature OnCurrentAmmoChangedDelegate;
	FOnIntValueChangedSignature OnMaxAmmoChangedDelegate;
	FOnActiveWeaponChangedSignature OnActiveWeaponChangedDelegate;
	FOnFloatValueChangedSignature OnAttackCooldownUpdatedDelegate;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UWeapon* GetActiveWeapon() { return ActiveWeapon; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Weapons")
	TObjectPtr<UWeapon> ActiveWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	TArray<TSubclassOf<UWeapon>> OwnedWeapons;
	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	void GiveNewWeapon(TSubclassOf<UWeapon> NewWeapon, bool bShouldEquipWeapon);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugAttackTraces;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPrimaryAttackInputDown")
	void OnPrimaryAttackInputDown_Event();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPrimaryAttackInputReleased")
	void OnPrimaryAttackInputReleased_Event();
	
	virtual bool PerformAttack();
	
	virtual void PerformReload();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPerformAttack();
	
	bool bIsPrimaryAttackInputHeld;
	
	float ActiveCooldown = 0.0f;
	float bIsReloading = false;
};
