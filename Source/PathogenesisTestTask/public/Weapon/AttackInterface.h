// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInfo.h"
#include "AttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};


class PATHOGENESISTESTTASK_API IAttackInterface
{
	GENERATED_BODY()

public:
	EWeaponType WeaponType;
	EWeaponPriority WeaponPriority;

	virtual void BeginAttack() = 0;
	virtual void ReleaseAttack() {}
};
