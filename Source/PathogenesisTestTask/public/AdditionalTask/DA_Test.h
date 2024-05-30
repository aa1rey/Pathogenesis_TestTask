// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Test.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESISTESTTASK_API UDA_Test : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|ItemInfo") FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|ItemInfo") float SomeFloatValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|ItemInfo") int32 SomeIntValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Service") bool bInitialized;
public:

	UDA_Test()
	{ 
		bInitialized = false; Name = ""; SomeFloatValue = 0.f; SomeIntValue = 0;
	}

	// Initializing values
	UFUNCTION(BlueprintCallable) void SetValues(FString str, float flt, int32 num)
	{
		bInitialized = true; Name = str; SomeFloatValue = flt; SomeIntValue = num;
	}

	// Returns false if not initialized. To initialize call "SetValues".
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetValues(FString& str, float& flt, int32& num)
	{ 
		if (!bInitialized) return false;
		str = Name; flt = SomeFloatValue; num = SomeIntValue;
		return true;
	}
};
