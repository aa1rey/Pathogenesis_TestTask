// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "AIState.generated.h"

namespace BBK
{
	TCHAR const* const TargetEnemy = TEXT("TargetEnemy");
	TCHAR const* const TargetLocation = TEXT("TargetLocation");
	TCHAR const* const State = TEXT("State");
}

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Idle = 0			UMETA(DisplayName = "Idle"),
	EAIS_Patrolling = 1		UMETA(DisplayName = "Patrolling"),
	EAIS_Attacking = 2		UMETA(DisplayName = "Attacking"),
	EAIS_Dead = 3			UMETA(DisplayName = "Dead"),
	EAIS_Investigating = 4	UMETA(DisplayName = "Investigating"),
};
