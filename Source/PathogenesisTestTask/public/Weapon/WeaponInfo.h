// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInfo.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_None		UMETA(DisplayName = "None"),
	EWT_Carbine		UMETA(DisplayName = "Carbine"),
	EWT_Pistol		UMETA(DisplayName = "Pistol"),
};

UENUM(BlueprintType)
enum class EWeaponPriority : uint8
{
	EWP_None = 0		UMETA(DisplayName = "None"),
	EWP_Primary = 1		UMETA(DisplayName = "Primary"),
	EWP_Secondary = 2	UMETA(DisplayName = "Secondary"),
};