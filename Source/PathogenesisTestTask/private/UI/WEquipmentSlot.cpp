// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WEquipmentSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "Inventory/InventoryComponent.h"
#include "Characters/EquipmentComponent.h"
#include "UI/DDInventory.h"
#include "Weapon/AttackInterface.h"
#include "Weapon/WeaponInfo.h"

using enum ESlateVisibility;

void UWEquipmentSlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	SizeBox->SetWidthOverride(HorizontalSize);
	SizeBox->SetHeightOverride(VerticalSize);
}

bool UWEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UDDInventory* DDInventory = Cast<UDDInventory>(InOperation);
	if (!DDInventory) return false;
	if (DDInventory->InventoryIndex == InventoryIndex) return false;
	
	FInventorySlot InvSlot;
	if (!InventoryRef->GetItemInfoAtIndex(DDInventory->InventoryIndex, InvSlot)) return false;
	if (!InvSlot.ItemInfo.bCanUse || (int32)InvSlot.ItemInfo.WeaponInfo.Priority == 0) return false;
	EWeaponPriority WeaponPriority = InvSlot.ItemInfo.WeaponInfo.Priority;
	if (EquipmentIndex + 1 != (int32)WeaponPriority) return false;

	EquipmentComponentRef->AddSlotAtIndex(EquipmentIndex, DDInventory->InventoryIndex);

	return true;
}

void UWEquipmentSlot::UpdateSlot(int32 EquipIndex, int32 InvIndex)
{
	if (!InventoryRef || !EquipmentComponentRef) return;
	if (EquipIndex != EquipmentIndex) return;
	InputText->SetText(FText::AsNumber(EquipmentIndex + 1));
	//EquipmentIndex = EquipIndex;
	InventoryIndex = InvIndex;

	FInventorySlot InvSlot;
	bool bSlotEmpty = !InventoryRef->GetItemInfoAtIndex(InventoryIndex, InvSlot);
	if (!bSlotEmpty) TextAmount->SetText(FText::AsNumber(InvSlot.Amount));

	SlotButton->SetIsEnabled(!bSlotEmpty);
	ItemIcon->SetVisibility(bSlotEmpty ? Hidden : HitTestInvisible);
	TextAmount->SetVisibility(bSlotEmpty ? Hidden : (InvSlot.ItemInfo.bCanStack ? HitTestInvisible : Hidden));
	ItemIcon->SetBrushFromTexture(bSlotEmpty ? nullptr : InvSlot.ItemInfo.Icon);
}