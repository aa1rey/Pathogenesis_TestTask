// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotUpdateSignature, int32, SlotIndex, int32, SlotValue);

// This component is an array of Inventory item indexes
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHOGENESISTESTTASK_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable) FOnSlotUpdateSignature OnSlotUpdate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 MaxAmount = 2;

protected:
	TArray<int32> Slots;
public:
	UEquipmentComponent();

	/*Add new equipment index to fast slots.
	@param	IndexToAdd	 - is index to add to fast slots.
	@param	IndexInArray - is index of a new fast equipment (if IndexToAdd doesn't exist in array)
	or index of existing equipment.	Check that the index is not equal to -1.
	@return	Returns true if the same IndexToAdd doesn't exist in array.*/
	UFUNCTION(BlueprintCallable) bool AddSlotAtIndex(int32 Index, int32 IndexToAdd);

	// Find given slot (IndexToRemove) in array and remove it.
	UFUNCTION(BlueprintCallable) bool RemoveSlot(int32 IndexToRemove);

	// Remove slot at the given Index in array.
	UFUNCTION(BlueprintCallable) bool RemoveSlotAtIndex(int32 Index);

	bool SwapSlots(int32 IndexA, int32 IndexB);

	// Returns index of given item (IndexToFind), if item doesn't exist in array returns -1.
	int32 FindEquipmentIndex(int32 IndexToFind) { for (int32 i = 0; i < MaxAmount; i++) if (Slots[i] == IndexToFind) return i; return -1; }

	int32 GetContentAtIndex(int32 Index) { return Slots[Index]; }

protected:
	virtual void BeginPlay() override;
	bool IsArrayFool() { return FindEquipmentIndex(-1) == -1; }
};
