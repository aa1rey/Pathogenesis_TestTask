// Fill out your copyright notice in the Description page of Project Settings.

// Components
#include "UI/WInventorySlot.h"
#include "UI/WInventory.h"
#include "UI/WInventoryDragDropSlot.h"
#include "UI/DDInventory.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

// Inventory
#include "Inventory/InventoryComponent.h"

// Service
#include "Kismet/GameplayStatics.h"

using enum ESlateVisibility;

void UWInventorySlot::UpdateSlot(int32 Index)
{
	SlotIndex = Index;
	FInventorySlot InvSlot;
	bool bSlotEmpty = !InventoryRef->GetItemInfoAtIndex(SlotIndex, InvSlot);
	if (!bSlotEmpty)
	{
		ItemInfo = InvSlot.ItemInfo;
		Amount = InvSlot.Amount;
		TextAmount->SetText(FText::AsNumber(Amount));
	}

	SlotButton->SetIsEnabled(!bSlotEmpty);
	ItemIcon->SetVisibility(bSlotEmpty ? Hidden : HitTestInvisible);
	TextAmount->SetVisibility(bSlotEmpty ? Hidden : (ItemInfo.bCanStack ? HitTestInvisible : Hidden));
	ItemIcon->SetBrushFromTexture(bSlotEmpty ? nullptr : ItemInfo.Icon);
}

void UWInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	SizeBox->SetWidthOverride(HorizontalSize);
	SizeBox->SetHeightOverride(VerticalSize);
}

FReply UWInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (SlotButton->GetIsEnabled() && SlotIndex > -1)
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) WInventory->ShowActionMenu(SlotIndex);

	return FReply::Handled();
}

FReply UWInventorySlot::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey /*|| PointerEvent.IsTouchEvent()*/)
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}

FReply UWInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UWInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!DragDropWidgetClass) { UE_LOG(LogTemp, Error, TEXT("DragDropWidgetClass is not valid!")); return; }
	if (UWInventoryDragDropSlot* DDWidget = CreateWidget<UWInventoryDragDropSlot>(this, DragDropWidgetClass))
	{
		DDWidget->Update(SlotIndex, InventoryRef);
		if (UDDInventory* DragDropOperation = NewObject<UDDInventory>())
		{
			DragDropOperation->DefaultDragVisual = DDWidget;
			DragDropOperation->InventoryIndex = SlotIndex;
			DragDropOperation->Inventory = InventoryRef;
			OutOperation = DragDropOperation;
		}
	}
}
