// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInfo.h"
#include "Logging/LogMacros.h"
#include "InventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdatedSignature, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryVisibilityUpdateSignature, bool, bVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUseSignature, FInventorySlot, Slot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHOGENESISTESTTASK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly) TArray<FInventorySlot> InvSlots;
	bool bIsVisible;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 SlotsAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 SlotsPerRow;
	UPROPERTY(BlueprintAssignable)	FOnSlotUpdatedSignature OnSlotUpdated;
	UPROPERTY(BlueprintAssignable)	FOnInventoryVisibilityUpdateSignature OnInventoryVisibilityUpdate;
	UPROPERTY(BlueprintAssignable)	FOnItemUseSignature OnItemUse;

public:	
	UInventoryComponent();

	// Toggle inventory visibility and broadcast about it. To subscribe use OnInventoryVisibilityUpdate delegate (Event Dispatcher)
	void ToggleInventoryVisibility() { bIsVisible = !bIsVisible; OnInventoryVisibilityUpdate.Broadcast(bIsVisible); }

	/*Replace existing Inventory Slot at the given Index with given Slot
	@return	Return true if given index is valid AND slot exists at this index, otherwise - false*/
	UFUNCTION(BlueprintCallable) bool UpdateSlotAtIndex(int32 Index, const FInventorySlot& Slot);

	/*Add to InventorySlots array a new item with given:
		* @param	ItemClass - class of item to store
		* @param	Amount - amount of items in this slot
		* @param	ItemInfo - info about Item in slot
		* @return	Returns true if item was added successfully and false if not*/
	bool AddItem(UClass* ItemClass, int32& Amount, const FInvItemInfo ItemInfo);

	/*Add to InvSlots array a new item with given InventorySlot structure
	* @return	Returns true if item was added successfully and false if not*/
	UFUNCTION(BlueprintCallable)
	bool AddItem(FInventorySlot& NewItem) { return AddItem(NewItem.ItemClass, NewItem.Amount, NewItem.ItemInfo); }

	/* Subtract given Amount of item at Index. If given Amount is equal to item amount,
	 clear slot without deleting it and shifting the indexing of the array.
	 @return	Returns true if subtracting is successful. */
	UFUNCTION(BlueprintCallable) bool RemoveItemAtIndex(int32 Index, int32 Amount);

	/* Clear slot without deleting it and shifting the indexing of the array.
	@return	Returns true if subtracting is successful.*/
	bool RemoveItemAtIndex(int32 Index) { return RemoveItemAtIndex(Index, InvSlots[Index].Amount); }

	// Returns false if there is no item at
	UFUNCTION(BlueprintCallable)
	bool GetItemInfoAtIndex(const int32 Index, FInventorySlot& Slot)
	{
		if (InvSlots.IsValidIndex(Index) && !IsEmptyslot(Index))
			Slot = InvSlots[Index];
		else return false;

		return true;
	}

	// @return If slot at the given index is empty - return true, otherwise - false
	UFUNCTION(BlueprintCallable) bool IsEmptyslot(int32 Index) { return InvSlots[Index].ItemClass == nullptr; }

	/* Find item by given class.
	* @param	TargetClass - item class that need to be found in inventory
	* @param	slot - info about found item
	* @param	Index - index in inventory, where the item was found
	* @return	If item by the TargetClass was found - return true, otherwise - false */
	UFUNCTION(BlueprintCallable) bool SearchItemByClass(const UClass* TargetClass, FInventorySlot& slot, int32& Index);

	bool UseItemAtIndex(int32 Index);
protected:
	virtual void BeginPlay() override;

	/* @param	ItemClass - item class of stack to be found
	* @param	Index - stack index that was found
	* @return	return true if stack was found, otherwise false */
	bool SearchFreeStack(const UClass* ItemClass, int32& Index);

	/* @param	Index - empty slot index that was found
	* @return	return true if empty slot was found otherwise - false */
	bool SearchEmptySlot(int32& Index);
};
