// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WInventoryDragDropSlot.generated.h"

class UInventoryComponent;

UCLASS()
class PATHOGENESISTESTTASK_API UWInventoryDragDropSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory") UInventoryComponent* InventoryRef;
	int32 SlotIndex;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	class UImage* ItemIcon;
public:
	void Update(int32 Index, UInventoryComponent* Inventory);
};
