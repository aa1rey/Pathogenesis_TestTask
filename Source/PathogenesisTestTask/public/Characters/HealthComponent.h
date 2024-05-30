// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentHealthUpdateSignarute, float, CurrHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHOGENESISTESTTASK_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable) FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable) FOnCurrentHealthUpdateSignarute OnCurrentHealthUpdate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float MaxHealth;

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetCurrentHealth() { return Health; }
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintCallable) void SetMaxHealth(float Value) { MaxHealth = Value; }

	// Use it only on game Load
	UFUNCTION(BlueprintCallable) void SetCurrentHealth(float Value) { Health = Value; OnCurrentHealthUpdate.Broadcast(Health); }

	UFUNCTION(BlueprintCallable) void AddHealth(float Value);
	UFUNCTION(BlueprintCallable) void SubHealth(float Value);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;	
};
