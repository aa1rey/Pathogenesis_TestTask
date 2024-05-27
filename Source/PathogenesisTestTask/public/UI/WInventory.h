// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WInventory.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemThrowSignature, int32 /*ItemIndex*/);

UCLASS(Abstract)
class PATHOGENESISTESTTASK_API UWInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")	class UInventoryComponent* InventoryRef;
	FOnItemThrowSignature OnItemThrow;
protected:
	UPROPERTY(EditDefaultsOnly)	TSubclassOf<UUserWidget> SlotWidgetClass;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UUniformGridPanel* InventorySlotsGrid;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UWActionMenu* ActionMenu;

public:
	/** Generating (spawning) an array of UW_InventorySlot objects on UniformGrindPanel, using data from UInventoryComponent object.
	* @return	Returs "false" if UInventoryComponent object isn't valid and "true" if generation was successful.*/
	UFUNCTION(BlueprintCallable) bool GenerateInventorySlots();
	UFUNCTION() void UpdateSlotAtIndex(int32 Index);
	void UpdateAllSlots();
	class UWInventorySlot* GetSlotWidgetAtIndex(int32 Index);
	void ShowActionMenu(int32 Index);

protected:
	virtual void NativeConstruct() override;
	UFUNCTION() void OnInventoryVisibilityUpdate(bool Visible);
	UFUNCTION()	void OnActionUse();
	UFUNCTION()	void OnActionThrow();
};
