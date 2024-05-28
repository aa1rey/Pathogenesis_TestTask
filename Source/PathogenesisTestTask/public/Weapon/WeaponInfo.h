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

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) EWeaponPriority Priority;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) EWeaponType Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) int32 MaxAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) int32 CurrentAmmo;

	FWeaponInfo() { Priority = EWeaponPriority::EWP_None; Type = EWeaponType::EWT_None; MaxAmmo = 0; CurrentAmmo = 0; }
	bool operator == (const FWeaponInfo& Info) const { return Priority == Info.Priority && Type == Info.Type; }
};