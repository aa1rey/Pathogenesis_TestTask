// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/BaseInteractive.h"
#include "ItemInfo.h"
#include "InventoryItem.generated.h"

UCLASS(Abstract)
class PATHOGENESISTESTTASK_API AInventoryItem : public ABaseInteractive
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly) FInventorySlot InventorySlot;
protected:
	//UPROPERTY(BlueprintReadWrite) class APlayerCharacter* PlayerRef;
public:	
	AInventoryItem();
protected:
	virtual void BeginPlay() override;

	virtual void StartInteraction_Implementation() override;
	virtual void HoldInteraction_Implementation() override { IInteractionInterface::Execute_StartInteraction(this); }
	virtual void StopInteraction_Implementation() override {}
};
