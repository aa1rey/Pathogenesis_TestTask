// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponInfo.h"
#include "ItemInfo.generated.h"


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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EWeaponPriority WeaponPriority;

	FInvItemInfo() { Icon = nullptr; bCanUse = false; bCanStack = false; bConsumable = false; MaxStackSize = 1; }
	bool operator == (const FInvItemInfo& Info) const
	{
		return Icon == Info.Icon
			&& bCanUse == Info.bCanUse
			&& bCanStack == Info.bCanStack
			&& bConsumable == Info.bConsumable 
			&& MaxStackSize == Info.MaxStackSize;
	}
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UClass* ItemClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 Amount = 1;
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