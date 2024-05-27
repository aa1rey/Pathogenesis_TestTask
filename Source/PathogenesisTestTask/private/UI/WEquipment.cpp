// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WEquipment.h"
#include "Components/HorizontalBox.h"
#include "UI/WEquipmentSlot.h"

#include "Characters/PlayerCharacter.h"
#include "Characters/EquipmentComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UWEquipment::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerRef) return;
	EquipmentComponentRef = PlayerRef->GetEquipmentComponent();
	InventoryComponentRef = PlayerRef->GetInventory();
	if (!EquipmentComponentRef || !InventoryComponentRef) return;
	GenerateSlots();
}

void UWEquipment::GenerateSlots()
{
	if (!EquipmentSlotWidgetClass) { UE_LOG(LogTemp, Error, TEXT("EquipmentSlotWidgetClass is not set in widget: '%s'"), *GetNameSafe(this)); return; }
	for (int32 i = 0; i < EquipmentComponentRef->MaxAmount; i++)
	{
		UWEquipmentSlot* NewWidget = CreateWidget<UWEquipmentSlot>(this, EquipmentSlotWidgetClass);
		NewWidget->InventoryRef = InventoryComponentRef;
		NewWidget->EquipmentComponentRef = EquipmentComponentRef;
		NewWidget->EquipmentIndex = i;
		NewWidget->UpdateSlot(-1);
		NewWidget->SetPadding(FMargin(10.f, 0.f));
		WeaponPanel->AddChild(NewWidget);
	}
}
