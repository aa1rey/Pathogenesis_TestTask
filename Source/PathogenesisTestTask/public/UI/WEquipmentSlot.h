// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WEquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESISTESTTASK_API UWEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	class UInventoryComponent* InventoryRef;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")	class UEquipmentComponent* EquipmentComponentRef;

	int32 InventoryIndex;
	int32 EquipmentIndex;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class USizeBox* SizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UButton* SlotButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UTextBlock* TextAmount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UImage* ItemIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UTextBlock* InputText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float HorizontalSize = 80.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float VerticalSize = 80.f;
public:
	UFUNCTION(BlueprintCallable) void UpdateSlot(int32 Index);
protected:
	virtual void NativePreConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
};
