// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/WeaponInfo.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UEquipmentComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitchSignature, EWeaponType, WeaponType);

UCLASS()
class PATHOGENESISTESTTASK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable) FOnWeaponSwitchSignature OnWeaponSwitch;

	UObject* InteractedObject;
	class IAttackInterface* WeaponRef;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Camera") USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Camera") UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Inventory") UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Inventory") UEquipmentComponent* EquipmentComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float MaxTargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float MinTargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float TargetArmScrollStep;

	UPROPERTY(EditDefaultsOnly) int32 EquipmentSlotsAmount;

	// ----------------------------------------- Input ----------------------------------------- //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ScrollCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectPrimaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSecondaryWeaponAction;
	// ----------------------------------------------------------------------------------------- //
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory") UInventoryComponent* GetInventory() const { return InventoryComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment") UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	bool CanInteract();
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION() void OnItemUse(UClass* ItemClass);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ScrollCamera(const FInputActionValue& Value);
	void Interact();
	void HoldInteract();
	void Inventory();
	void BeginAttack();
	void ReleaseAttack();
	void SelectWeapon(int32 Index);
};
