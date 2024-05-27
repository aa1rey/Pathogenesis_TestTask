// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemInfo.h"
#include "WActionMenu.generated.h"

class UButton;

UCLASS(Abstract)
class PATHOGENESISTESTTASK_API UWActionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FInvItemInfo ItemInfo;
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	UButton* ThrowButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	UButton* FastEquipButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))	UButton* CancelButton;

protected:
	UPROPERTY(EditDefaultsOnly)	TSubclassOf<UUserWidget> FastEquipmentWidgetClass;
public:
	void Update(int32 Index);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()	void OnActionCancel() { SetVisibility(ESlateVisibility::Hidden); }
	UFUNCTION()	void OnActionEquipToSlot();
};
