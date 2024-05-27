// Fill out your copyright notice in the Description page of Project Settings.

// Components
#include "UI/WInventory.h"
#include "UI/WInventorySlot.h"
#include "UI/WActionMenu.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"

// Inventory
#include "Inventory/ItemInfo.h"
#include "Characters/PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"

// Service
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/UniformGridSlot.h"

using enum ESlateVisibility;


void UWInventory::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;
	InventoryRef = player->GetInventory();
	// OnItemThrow.BindUObject(player, &APlayerCharacter::OnItemThrow);
	InventoryRef->OnSlotUpdated.AddDynamic(this, &ThisClass::UpdateSlotAtIndex);
	InventoryRef->OnInventoryVisibilityUpdate.AddDynamic(this, &ThisClass::OnInventoryVisibilityUpdate);
	GenerateInventorySlots();

	ActionMenu->UseButton->OnClicked.AddDynamic(this, &ThisClass::OnActionUse);
	ActionMenu->ThrowButton->OnClicked.AddDynamic(this, &ThisClass::OnActionThrow);
}

void UWInventory::OnInventoryVisibilityUpdate(bool Visible)
{
	SetVisibility(Visible ? SelfHitTestInvisible : Hidden);
}

void UWInventory::OnActionUse()
{
	InventoryRef->UseItemAtIndex(ActionMenu->SlotIndex);
	ActionMenu->SetVisibility(Hidden);
}

void UWInventory::OnActionThrow()
{
	if (!InventoryRef->RemoveItemAtIndex(ActionMenu->SlotIndex)) return;
	OnItemThrow.Execute(ActionMenu->SlotIndex);
	ActionMenu->SetVisibility(Hidden);
}

bool UWInventory::GenerateInventorySlots()
{
	if (!InventoryRef) return false;
	int32 SlotsAmount = InventoryRef->SlotsAmount;
	int32 SlotsPerRow = InventoryRef->SlotsPerRow;

	InventorySlotsGrid->ClearChildren();
	for (int Row = 0; Row < SlotsAmount / SlotsPerRow; Row++)
	{
		for (int Col = 0; Col < SlotsPerRow; Col++)
		{
			check(SlotWidgetClass);
			if (UWInventorySlot* SlotWidget = Cast<UWInventorySlot>(CreateWidget<UUserWidget>(this, SlotWidgetClass)))
			{
				SlotWidget->InventoryRef = InventoryRef;
				InventorySlotsGrid->AddChildToUniformGrid(SlotWidget, Row, Col);
				SlotWidget->UpdateSlot(-1);
				SlotWidget->WInventory = this;
			}
		}
	}

	return true;
}

void UWInventory::UpdateSlotAtIndex(int32 Index)
{
	UWInventorySlot* SlotWidget = Cast<UWInventorySlot>(InventorySlotsGrid->GetAllChildren()[Index]);
	SlotWidget->UpdateSlot(Index);
}

void UWInventory::UpdateAllSlots()
{
	for (int i = 0; i < InventoryRef->SlotsAmount; i++)
		UpdateSlotAtIndex(i);
}

UWInventorySlot* UWInventory::GetSlotWidgetAtIndex(int32 Index)
{
	if (Index > -1 && Index < InventorySlotsGrid->GetChildrenCount())
		return Cast<UWInventorySlot>(InventorySlotsGrid->GetChildAt(Index));
	return nullptr;
}

void UWInventory::ShowActionMenu(int32 Index)
{
	UWInventorySlot* WSlot = Cast<UWInventorySlot>(InventorySlotsGrid->GetChildAt(Index));

	FVector2D Translation;
	float GridSlotPadding = InventorySlotsGrid->GetSlotPadding().Bottom * 2;
	Translation.X = UWidgetLayoutLibrary::SlotAsUniformGridSlot(WSlot)->GetColumn() * (WSlot->HorizontalSize + GridSlotPadding);
	Translation.Y = UWidgetLayoutLibrary::SlotAsUniformGridSlot(WSlot)->GetRow() * (WSlot->VerticalSize + GridSlotPadding);
	ActionMenu->SetRenderTranslation(Translation);

	FInventorySlot slot;
	if (InventoryRef->GetItemInfoAtIndex(Index, slot))
		ActionMenu->ItemInfo = slot.ItemInfo;

	ActionMenu->Update(WSlot->SlotIndex);
	ActionMenu->SetVisibility(SelfHitTestInvisible);
}
