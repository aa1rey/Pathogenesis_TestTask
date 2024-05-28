// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackInterface.h"
#include "ReloadInterface.h"
#include "Inventory/UseInterface.h"
#include "FirearmsWeapon.generated.h"

class UBarrel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInfoUpdateSignature, FWeaponInfo, WeaponInfo);

UCLASS()
class PATHOGENESISTESTTASK_API AFirearmsWeapon : public AActor,
	public IAttackInterface,
	public IUseInterface,
	public IReloadInterface
{
	GENERATED_BODY()

public:
	AActor* OwnerActor;
	UPROPERTY(BlueprintAssignable) FOnInfoUpdateSignature OnInfoUpdate;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USceneComponent* SceneRoot;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USkeletalMeshComponent* MeshComponent;

	// SkeletalMeshComponent is better suited, but StaticMeshComponent will be sufficient for this test task
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USceneComponent* MuzzleFlashSocket;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UBarrel* BarrelComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attachment") FName AttachSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Info") FWeaponInfo Info;
	FText Name;

public:	
	AFirearmsWeapon();
	virtual UBarrel* GetBarrelComponent() const override { return BarrelComponent; }
protected:
	virtual void BeginPlay() override;
	virtual void BeginAttack() override;
	virtual void ReleaseAttack() override;
	virtual FWeaponInfo GetWeaponInfo() override { return Info; }
	virtual void Reload() override;
	virtual void Use(class ACharacter* OwnerRef, FInventorySlot Slot) override;

	UFUNCTION() void OnSingleChargeEvent(int32 CurrentAmmoAmount);
	UFUNCTION() void OnChargeCompetedEvent(int32 CurrentAmmoAmount, int32 RestedAmmo);
};
