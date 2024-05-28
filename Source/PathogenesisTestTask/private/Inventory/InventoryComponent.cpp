// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogInventory);

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SlotsAmount = 5;
	bIsVisible = false;
}

bool UInventoryComponent::UpdateSlotAtIndex(int32 Index, const FInventorySlot& Slot)
{
	if (!InvSlots.IsValidIndex(Index) || IsEmptyslot(Index)) return false;
	
	InvSlots[Index] = Slot;
	return true;
}

bool UInventoryComponent::AddItem(UClass* ItemClass, int32& Amount, const FInvItemInfo ItemInfo)
{
	if (InvSlots.Num() == 0)
	{
		UE_LOG(LogInventory, Error, TEXT("Error! Zero inventory size of '%s'!"), *GetNameSafe(GetOwner()));
		return false;
	}
	if (Amount == 0 || ItemClass == nullptr)
	{
		UE_LOG(LogInventory, Warning, TEXT("Warring! Adding Amount value is zero or ItemClass is nullptr! Amount: '%i'. ItemClass: '%s'"), Amount, *GetNameSafe(ItemClass));
		return false;
	}

	bool bSuccess = false;
	int32 Local_FoundIndex;

	if (ItemInfo.bCanStack)
	{
		if (SearchFreeStack(ItemClass, Local_FoundIndex))
		{
			if (InvSlots[Local_FoundIndex].Amount + Amount <= InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize)
			{
				InvSlots[Local_FoundIndex].Amount += Amount;
				Amount = 0;
				bSuccess = true;
				OnSlotUpdated.Broadcast(Local_FoundIndex);
			}
			else
			{
				int32 Local_Rest = InvSlots[Local_FoundIndex].Amount + Amount - InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize;
				Amount = Local_Rest;
				InvSlots[Local_FoundIndex].Amount = InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize;
				bSuccess = true;
				OnSlotUpdated.Broadcast(Local_FoundIndex);
				AddItem(ItemClass, Amount, ItemInfo);
			}
		}
		else
		{
			if (SearchEmptySlot(Local_FoundIndex))
			{
				InvSlots[Local_FoundIndex].ItemClass = ItemClass;
				InvSlots[Local_FoundIndex].ItemInfo = ItemInfo;

				if (Amount <= InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize)
				{
					InvSlots[Local_FoundIndex].Amount = Amount;
					Amount = 0;
					bSuccess = true;
					OnSlotUpdated.Broadcast(Local_FoundIndex);
				}
				else
				{
					InvSlots[Local_FoundIndex].Amount = InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize;
					int32 Local_Rest = Amount - InvSlots[Local_FoundIndex].ItemInfo.MaxStackSize;
					Amount = Local_Rest;
					bSuccess = true;
					OnSlotUpdated.Broadcast(Local_FoundIndex);
					AddItem(ItemClass, Amount, ItemInfo);
				}
			}
			else bSuccess = false;
		}
	}
	else
	{
		if (SearchEmptySlot(Local_FoundIndex))
		{
			InvSlots[Local_FoundIndex].ItemClass = ItemClass;
			InvSlots[Local_FoundIndex].Amount = 1;
			InvSlots[Local_FoundIndex].ItemInfo = ItemInfo;
			OnSlotUpdated.Broadcast(Local_FoundIndex);
			Amount--;
			if (Amount > 0) AddItem(ItemClass, Amount, ItemInfo);
			else bSuccess = true;
		}
	}

	return bSuccess;
}

bool UInventoryComponent::RemoveItemAtIndex(int32 Index, int32 Amount)
{
	if (IsEmptyslot(Index) || InvSlots[Index].Amount < Amount) return false;

	if (InvSlots[Index].Amount == Amount)
	{
		InvSlots[Index].ItemClass = nullptr;
		InvSlots[Index].ItemInfo.Icon = nullptr;
	}
	InvSlots[Index].Amount -= Amount;
	OnSlotUpdated.Broadcast(Index);

	return true;
}

bool UInventoryComponent::SearchItemByClass(const UClass* TargetClass, FInventorySlot& slot, int32& Index)
{
	if (TargetClass != nullptr)
		for (int32 i = 0; i < InvSlots.Num(); i++)
			if (InvSlots[i].ItemClass == TargetClass) { slot = InvSlots[i]; Index = i; return true; }

	return false;
}

bool UInventoryComponent::UseItemAtIndex(int32 Index)
{
	FInventorySlot slot;
	if (!GetItemInfoAtIndex(Index, slot)) { UE_LOG(LogInventory, Error, TEXT("Error! Can not use item at index, because either index is invalid or Slot is empty!")); return false; }
	if (!slot.ItemInfo.bCanUse) { UE_LOG(LogInventory, Warning, TEXT("Can not use item, because its bCanUse is false!")); return false; }

	OnItemUse.Broadcast(slot);
	if (slot.ItemInfo.bConsumable)
		RemoveItemAtIndex(Index, 1);

	return true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InvSlots.SetNumZeroed(SlotsAmount);
	
}

bool UInventoryComponent::SearchFreeStack(const UClass* ItemClass, int32& Index)
{
	for (Index = 0; Index < InvSlots.Num(); Index++)
		if (InvSlots[Index].ItemClass == ItemClass && InvSlots[Index].Amount < InvSlots[Index].ItemInfo.MaxStackSize)
			return true;

	return false;
}

bool UInventoryComponent::SearchEmptySlot(int32& Index)
{
	for (Index = 0; Index < InvSlots.Num(); Index++)
		if (InvSlots[Index].ItemClass == nullptr)
			return true;

	return false;
}
