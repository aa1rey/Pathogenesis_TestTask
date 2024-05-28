// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponInfo.h"
#include "ItemInfo.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Default = 0		UMETA(DisplayName = "Default"),
	EIT_Weapon = 1		UMETA(DisplayName = "Weapon"),
};

USTRUCT(BlueprintType)
struct FInvItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bCanUse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bCanStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bConsumable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 MaxStackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FWeaponInfo WeaponInfo;

	FInvItemInfo() { Icon = nullptr; bCanUse = false; bCanStack = false; bConsumable = false; MaxStackSize = 1; ItemType = EItemType::EIT_Default; }
	bool operator == (const FInvItemInfo& Info) const
	{
		return Icon == Info.Icon
			&& bCanUse == Info.bCanUse
			&& bCanStack == Info.bCanStack
			&& bConsumable == Info.bConsumable 
			&& MaxStackSize == Info.MaxStackSize
			&& ItemType == Info.ItemType
			&& ItemType == Info.ItemType
			&& WeaponInfo == Info.WeaponInfo;
	}
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UClass* ItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) int32 Amount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FInvItemInfo ItemInfo;

	FInventorySlot() { ItemClass = nullptr; Amount = 0; }
	bool operator == (const FInventorySlot& Slot) const
	{
		return 
			ItemClass == Slot.ItemClass && 
			Amount == Slot.Amount && 
			ItemInfo == Slot.ItemInfo;
	}
};