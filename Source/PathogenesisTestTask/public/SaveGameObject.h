// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESISTESTTASK_API USaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TMap<FString, FVector> CharactersLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TMap<FString, float> CharactersHealth;
};
