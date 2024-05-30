// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WActionMenu.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

using enum ESlateVisibility;

void UWActionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &ThisClass::OnActionCancel);

	SetVisibility(Hidden);
}

void UWActionMenu::Update(int32 Index)
{
	SlotIndex = Index;
	UseButton->SetVisibility(ItemInfo.bCanUse ? Visible : Collapsed);
}