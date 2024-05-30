// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthInterface.h"
#include "SaveLoadInterface.h"
#include "Weapon/WeaponInfo.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
//class UInventory;
class UEquipmentComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitchSignature, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSelectSignature, int32, priority, bool, Selected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInfoUpdateSignature, FWeaponInfo, WeaponInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrientToMousePositionSignature, bool, bOrient);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseSignature, bool, bPause);

UCLASS()
class PATHOGENESISTESTTASK_API APlayerCharacter : public ACharacter, public IHealthInterface, public ISaveLoadInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable) FOnWeaponSwitchSignature OnWeaponSwitch;
	UPROPERTY(BlueprintAssignable) FOnWeaponSelectSignature OnWeaponSelect;

	UPROPERTY(BlueprintAssignable) FOnWeaponInfoUpdateSignature OnWeaponInfoUpdate;

	UPROPERTY(BlueprintAssignable) FOnOrientToMousePositionSignature OnOrientToMousePosition;

	UPROPERTY(BlueprintAssignable) FOnPauseSignature OnPause;

	UObject* InteractedObject;
	class IAttackInterface* WeaponRef;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Camera") USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Camera") UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Inventory") UInventoryComponent* InventoryComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|OpenInventory") UInventory* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|OpenInventory") UEquipmentComponent* EquipmentComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Health") class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float MaxTargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float MinTargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Components|Camera") float TargetArmScrollStep;

	UPROPERTY(EditDefaultsOnly) int32 EquipmentSlotsAmount;

	/* Array of inventory item classes that are used to heal a character.
	Add here only those classes that must be stored in the inventory,
	otherwise they won't be found in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OpenInventory|Heal") TArray<UClass*> HealItemClasses;

	bool bOrientToCursor;
	bool bRotateCameraKeyDown;
	bool bGamePaused;

	// ----------------------------------------- Input  Actions ----------------------------------------- //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateCameraAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HealAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	// -------------------------------------------------------------------------------------------------- //

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory") UInventoryComponent* GetInventory() const { return InventoryComponent; }
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "OpenInventory") UInventory* GetInventory() const { return Inventory; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment") UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health") UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	bool CanInteract();
	void OnItemThrow(int32 ItemIndex);
	UFUNCTION(BlueprintCallable) void TogglePause();
protected:
	virtual void BeginPlay() override;
	UFUNCTION() void OnItemUse(FInventorySlot Slot);
	UFUNCTION() void OnWeaponCurrentAmountUpdateEvent(int32 Amount);
	UFUNCTION(BlueprintNativeEvent) void OnDeath();
	void OnDeath_Implementation();
	virtual void AddHealth_Implementation(float Value) override;
	virtual void SubHealth_Implementation(float Value) override;


	// ----------------------------------------- Input  Events ----------------------------------------- //
	void Move(const FInputActionValue& Value);
	void RotateCamera(bool bDown) { bRotateCameraKeyDown = bDown; }
	void Look(const FInputActionValue& Value);
	void ScrollCamera(const FInputActionValue& Value);
	void Interact();
	void HoldInteract();
	void OpenInventory();
	void BeginAttack();
	void ReleaseAttack();
	void SelectWeapon(int32 Index);
	void ReloadWeapon();
	void Heal();
	void Pause() { TogglePause(); }
	// ------------------------------------------------------------------------------------------------- //

	virtual bool Save_Implementation() override;
	virtual bool Load_Implementation() override;
};
