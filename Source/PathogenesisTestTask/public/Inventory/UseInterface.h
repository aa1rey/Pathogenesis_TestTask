// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PATHOGENESISTESTTASK_API IUseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Use|From Use Interface") void Use(class ACharacter* OwnerRef, FInventorySlot Slot);
	
	virtual void Use_Implementation(class ACharacter* OwnerRef, FInventorySlot Slot) = 0;
};
