// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Health = MaxHealth = 100.f;
}


void UHealthComponent::AddHealth(float Value)
{
	if (Health == 0.f) return;
	if (Health + Value < MaxHealth) Health += Value;
	else Health = MaxHealth;
	OnCurrentHealthUpdate.Broadcast(Health);
}

void UHealthComponent::SubHealth(float Value)
{
	if (Health - Value > 0.f) Health -= Value;
	else { Health = 0.f; OnDeath.Broadcast(); }
	OnCurrentHealthUpdate.Broadcast(Health);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}