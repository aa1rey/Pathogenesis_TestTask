// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Barrel.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBarrel, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShootFiredSignature, FVector, Velocity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChargeCompletedSignature, int32, CurrAmmoAmount, int32, RestedAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentAmmoUpdateSugnature, int32, CurrAmmoAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfAmmoSignature);

UENUM(BlueprintType)
enum class EFiringType : uint8
{
	// Full auto - The Barrel will fire until the Shoot Event is triggered as completed(false)
	EFT_FullAuto = 0			UMETA(DisplayName = "Full Auto"),
	// The Barrel will fire once each time the Shoot Event is triggered as started (true)
	EFT_SemiAuto = 1			UMETA(DisplayName = "Semi Auto"),
	// The Barrel will fire with specified BurstAmount each time the Shoot Event is triggered as started(true). If Shoot event is triggered with false, won't be interrupted.
	EFT_Burst = 2				UMETA(DisplayName = "Burst"),
	// Simular to the Burst, but if Shoot event is triggered with false, will be interrupted.
	EFT_InterruptedBurst = 3	UMETA(DisplayName = "Interrupted Burst"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHOGENESISTESTTASK_API UBarrel : public USceneComponent
{
	GENERATED_BODY()

public:

	// Delegate (Event Dispatcher) that makes a call every time the Barrel shoots
	UPROPERTY(BlueprintAssignable) FOnShootFiredSignature OnShootFired;

	// Delegate (Event Dispatcher) that makes a call every single charge
	UPROPERTY(BlueprintAssignable) FOnCurrentAmmoUpdateSugnature OnCurrentAmmoUpdate;

	// Delegate (Event Dispatcher) that makes a call when charging is completed. Notifies about the remaining number of ammo after recharging
	UPROPERTY(BlueprintAssignable) FOnChargeCompletedSignature OnChargeCompleted;

	// Delegate (Event Dispatcher) that makes a call when trying to shoot with zero Ammo amount
	UPROPERTY(BlueprintAssignable) FOnOutOfAmmoSignature OnOutOfAmmo;

	// Actors which wil be ignored when firing
	UPROPERTY(BlueprintReadWrite, Category = "Barrel|Bullet|Debug") TArray<AActor*> BulletIgnoreActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Ammo") bool bInfiniteAmmo;
	AActor* DamageCauser;
protected:
	// Class of bullet that will be fired. Use class inherited from Bullet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") TSubclassOf<class ABullet> BulletClass;
	// The time after which the bullet will be destroyed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") float BulletLifeTime;

	// The time after Shoot Event is triggered when the bullet will be fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") float InitialFiringDelay;

	// Minimal initial velocity of the bullet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") float MuzzleVelocityMin;

	// Maximal initial velocity of the bullet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") float MuzzleVelocityMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel") EFiringType FiringType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "FiringType==EFiringType::EFT_Burst||FiringType==EFiringType::EFT_InterruptedBurst", EditConditionHides), Category = "Barrel") int32 BurstAmount;

	// Minimal time (in seconds) at which the shot will be fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel") float FireRateMin;
	// Maximal time (in seconds) at which the shot will be fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel") float FireRateMax;

	// Overrides a bullet damage. If is equel to 0, damage won't be overrided
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet") float OverrideBulletDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") bool bTraceBulletWithCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") bool bTraceBulletWithChannel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") bool bTraceBulletWithObjectTypes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") bool bBulletTraceComplex;

	// Determines size of each sub-step in the simulation. Recommended between 10 to 30 depending on desired quality versus performance.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") int32 SimFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Bullet|Debug") TEnumAsByte<EDrawDebugTrace::Type> BulletTraceDebugType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Bullet Trace Channel", EditCondition = "bTraceBulletWithChannel", EditConditionHides), Category = "Barrel|Bullet|Debug")
	TEnumAsByte<ECollisionChannel> BulletTraceChannel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Bullet Object Types", EditCondition = "bTraceBulletWithObjectTypes", EditConditionHides), Category = "Barrel|Bullet|Debug")
	TArray<TEnumAsByte<EObjectTypeQuery>> BulletObjectTypes;

	FTimerHandle ShootHandle, ChargeHandle;
	int32 BurstIterator, SpecifiedCharges;
	bool bCanShoot;

	// Magazine size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Ammo") int32 MaxAmmoAmount;
	
	// The time required to charge one ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Ammo") float ChargeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel|Ammo") int32 CurrentAmmoAmount;
	
public:	
	UBarrel();

	/* Start or end the firing event depending on the passed parameter
	* @param	Trigger - determines the start or end of shooting*/
	UFUNCTION(BlueprintCallable) void Shoot(bool Trigger);

	// Returns the current ammo amount
	UFUNCTION(BlueprintCallable, BlueprintPure) int32 GetCurrentAmmoAmount() { return CurrentAmmoAmount; }
	UFUNCTION(BlueprintCallable, BlueprintPure) int32 GetMaxAmmoAmount() { return MaxAmmoAmount; }

	void SetMaxAmmoAmount(int32 NewAmount) { MaxAmmoAmount = NewAmount; }
	void SetCurrentAmmoAmount(int32 NewAmount) { CurrentAmmoAmount = NewAmount; }
	
	/* Charge specified AmmoAmount
	* @param	AmmoAmount - number of ammo to charge
	* @param	bImmediately - if true, charge without time delay, won't broadcast OnCurrentAmmoUpdate delegate.
	* Otherwise OnChargeCompleted will make a call, when charge is completed, and OnCurrentAmmoUpdate will make a call every single charge.
	* @param	OverrideChargeTime - the time required to charge one ammo. If lower than 0, won't override ChargeTime defined in class.*/
	UFUNCTION(BlueprintCallable) void Charge(int32 AmmoAmount, bool bImmediately = true, float OverrideChargeTime = -1.f);
	void ShootFired();
protected:
	virtual void BeginPlay() override;
	void EndShoot();
	void SingleCharge();
	void EndCharge();
};
