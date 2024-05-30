// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveLoadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveLoadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PATHOGENESISTESTTASK_API ISaveLoadInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Save and Load|From SaveLoadInterface") bool Save();
	virtual bool Save_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Save and Load|From SaveLoadInterface") bool Load();
	virtual bool Load_Implementation() = 0;
};
