// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WEquipment.generated.h"

class UHorizontalBox;

UCLASS()
class PATHOGENESISTESTTASK_API UWEquipment : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	UHorizontalBox* WeaponPanel;

	UPROPERTY(BlueprintReadOnly) class UEquipmentComponent* EquipmentComponentRef;
	UPROPERTY(BlueprintReadOnly) class UInventoryComponent* InventoryComponentRef;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> EquipmentSlotWidgetClass;
protected:
	virtual void NativeConstruct() override;
	void GenerateSlots();
};
