// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESISTESTTASK_API USaveGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

	template <class T>
	void CreateSaveFile(FString SlotName)
	{
		T* SaveData = Cast<T>(UGameplayStatics::CreateSaveGameObject(T::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveData, SlotName, 0);
	}

	template <class T>
	inline T* GetSaveGameObject(FString SlotName)
	{
		return Cast<T>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}

	UFUNCTION(BlueprintCallable) void ClearAllSaves(FString SlotName);

	UFUNCTION(BlueprintCallable) void SaveLocation(const FString& CharacterName, FVector Location);
	UFUNCTION(BlueprintCallable) bool LoadLocation(const FString& CharacterName, FVector& Location);

	UFUNCTION(BlueprintCallable) void SaveHealth(const FString& CharacterName, float Health);
	UFUNCTION(BlueprintCallable) bool LoadHealth(const FString& CharacterName, float& Health);
};
