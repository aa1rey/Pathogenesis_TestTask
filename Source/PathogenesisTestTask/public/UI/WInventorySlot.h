// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemInfo.h"
#include "WInventorySlot.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PATHOGENESISTESTTASK_API UWInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	FInvItemInfo ItemInfo;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	int32 Amount;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	int32 SlotIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	class UInventoryComponent* InventoryRef;
	UPROPERTY(BlueprintReadWrite, Category = "WBPs")		class UWInventory* WInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float HorizontalSize = 80.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float VerticalSize = 80.f;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class USizeBox* SizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UButton* SlotButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UTextBlock* TextAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UImage* ItemIcon;

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> OwnerWidgetClass;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> DragDropWidgetClass;
public:

	/** This event is called from BaseInventory class after any item
	* would be added to BaseInventory. Uses to update item info
	* at Widget slot (item image, name, description, etc...). */
	UFUNCTION(BlueprintCallable) void UpdateSlot(int32 Index);

protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
};
