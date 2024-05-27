// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEquipmentComponent::AddSlotAtIndex(int32 Index, int32 IndexToAdd)
{
	if (!Slots.IsValidIndex(Index)) return false;

	int32 foundIndex = FindEquipmentIndex(IndexToAdd);
	if (foundIndex != -1) return SwapSlots(Index, foundIndex);

	Slots[Index] = IndexToAdd;
	OnSlotUpdate.Broadcast(Index, IndexToAdd);
	return true;
}

bool UEquipmentComponent::RemoveSlot(int32 IndexToRemove)
{
	int32 i = FindEquipmentIndex(IndexToRemove);
	if (!Slots.IsValidIndex(i)) return false;
	return RemoveSlotAtIndex(i);
}

bool UEquipmentComponent::RemoveSlotAtIndex(int32 Index)
{
	if (!Slots.IsValidIndex(Index)) return false;
	Slots[Index] = -1;

	return true;
}

bool UEquipmentComponent::SwapSlots(int32 IndexA, int32 IndexB)
{
	if (!Slots.IsValidIndex(IndexA) || !Slots.IsValidIndex(IndexB)) return false;

	int32 temp = Slots[IndexB];
	Slots[IndexB] = Slots[IndexA];
	Slots[IndexA] = temp;

	OnSlotUpdate.Broadcast(IndexA, Slots[IndexA]);
	OnSlotUpdate.Broadcast(IndexB, Slots[IndexB]);

	return true;
}


void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// Setting number of array elements and giving them -1 value
	for (int32 i = 0; i < MaxAmount; i++) Slots.Add(-1);
}