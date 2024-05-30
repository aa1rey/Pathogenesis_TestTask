// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthInterface.h"
#include "SaveLoadInterface.h"
#include "BaseEnemy.generated.h"

UCLASS()
class PATHOGENESISTESTTASK_API ABaseEnemy : public ACharacter, public IHealthInterface, public ISaveLoadInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrolling") class APatrolPath* PatrolPath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components|Health") class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") FName AttachWeaponSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") UClass* WeaponClass;
	class IAttackInterface* WeaponRef;
	AActor* TargetEnemy;

public:
	ABaseEnemy();
	UFUNCTION() virtual void OnDeath();

	UFUNCTION(BlueprintCallable) virtual void BeginAttack();
	UFUNCTION(BlueprintCallable) virtual void EndAttack();
protected:
	virtual void BeginPlay() override;
	virtual void AddHealth_Implementation(float Value) override;
	virtual void SubHealth_Implementation(float Value) override;

	virtual bool Save_Implementation() override;
	virtual bool Load_Implementation() override;
};
