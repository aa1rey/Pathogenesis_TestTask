// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/InventoryComponent.h"
#include "DDInventory.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESISTESTTASK_API UDDInventory : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite) UInventoryComponent* Inventory;
	UPROPERTY(BlueprintReadWrite) int32 InventoryIndex;
	UPROPERTY(BlueprintReadWrite) int32 EquipmentIndex;
};
