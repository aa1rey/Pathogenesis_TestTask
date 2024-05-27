// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WInventoryDragDropSlot.h"
#include "Components/Image.h"
#include "Inventory/InventoryComponent.h"

void UWInventoryDragDropSlot::Update(int32 Index, UInventoryComponent* Inventory)
{
	if (!Inventory) return;
	InventoryRef = Inventory;
	SlotIndex = Index;

	FInventorySlot slot;
	if (!InventoryRef->GetItemInfoAtIndex(SlotIndex, slot)) return;
	ItemIcon->SetBrushFromTexture(slot.ItemInfo.Icon);
}
