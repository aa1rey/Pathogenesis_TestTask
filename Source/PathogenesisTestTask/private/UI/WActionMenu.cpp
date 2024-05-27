// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WActionMenu.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

using enum ESlateVisibility;

void UWActionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &ThisClass::OnActionCancel);
	FastEquipButton->OnClicked.AddDynamic(this, &ThisClass::OnActionEquipToSlot);

	SetVisibility(Hidden);
}

void UWActionMenu::OnActionEquipToSlot()
{
	if (!FastEquipmentWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Error! FastEquipmentWidgetClass is not set in widget: '%s'!"), *GetNameSafe(this));
		return;
	}

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, FastEquipmentWidgetClass, false);

	if (FoundWidgets.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning! Can not find widget of class: '%s'!"), *GetNameSafe(FastEquipmentWidgetClass));
		return;
	}

	/*if (UWEquipmentWheel* EquipWheelWidget = Cast<UWEquipmentWheel>(FoundWidgets[0]))
	{
		EquipWheelWidget->BeginAddToFastSlot(SlotIndex);
		SetVisibility(Hidden);
	}*/
}

void UWActionMenu::Update(int32 Index)
{
	SlotIndex = Index;
	UseButton->SetVisibility(ItemInfo.bCanUse ? Visible : Collapsed);
	FastEquipButton->SetVisibility(ItemInfo.bCanUse ? Visible : Collapsed);
}