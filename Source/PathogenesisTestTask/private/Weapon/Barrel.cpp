// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Barrel.h"
#include "Weapon/Bullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogBarrel);

using enum EFiringType;

UBarrel::UBarrel()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	bCanShoot = true;
	BurstIterator = 0;
	InitialAmmoAmount = 0;
	CurrentAmmoAmount = InitialAmmoAmount;
	ChargeTime = 0.f;
	SpecifiedCharges = 0;
	BulletClass = ABullet::StaticClass();
	InitialFiringDelay = 0.f;
	FireRateMin = 1.f;
	FireRateMax = 1.f;
	MuzzleVelocityMin = 1000.f;
	MuzzleVelocityMax = 1000.f;
	SimFrequency = 15;
	bTraceBulletWithCollision = true;
	bTraceBulletWithChannel = true;
	BulletTraceChannel = ECC_WorldStatic;
	bTraceBulletWithObjectTypes = false;
	bBulletTraceComplex = false;
	BulletLifeTime = 5.f;
	BulletTraceDebugType = EDrawDebugTrace::ForDuration;
}

void UBarrel::Shoot(bool Trigger)
{
	if (Trigger)
	{
		if (!bCanShoot) return;

		if (!BulletClass)
		{
			UE_LOG(LogBarrel, Error, TEXT("Error! BulletClass is None!"));
			return;
		}

		bool bLooped = true;
		if (FiringType == EFT_SemiAuto) bLooped = false;

		float FireRate = UKismetMathLibrary::RandomFloatInRange(FireRateMin, FireRateMax);
		GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &UBarrel::ShootFired, FireRate, bLooped, InitialFiringDelay);
	}
	else if (FiringType != EFT_Burst) EndShoot();
}

void UBarrel::Charge(int32 AmmoAmount, bool bImmediately, float OverrideChargeTime)
{
	if (AmmoAmount < 1 || bInfiniteAmmo) return;

	if (bImmediately)
	{
		if (CurrentAmmoAmount + AmmoAmount <= MaxAmmoAmount)
			CurrentAmmoAmount += AmmoAmount;
		else
		{
			float rest = CurrentAmmoAmount + AmmoAmount - MaxAmmoAmount;
			CurrentAmmoAmount = MaxAmmoAmount;
			OnChargeCompleted.Broadcast(rest);
		}
		bCanShoot = CurrentAmmoAmount > 0;
	}
	else
	{
		SpecifiedCharges = AmmoAmount + CurrentAmmoAmount;
		GetWorld()->GetTimerManager().SetTimer(ChargeHandle, this, &UBarrel::SingleCharge, OverrideChargeTime < 0 ? ChargeTime : OverrideChargeTime, true);
	}
}

void UBarrel::ShootFired()
{
	if (!bInfiniteAmmo)
	{
		if (CurrentAmmoAmount > 0) CurrentAmmoAmount--;
		else { bCanShoot = false; EndShoot(); return; }
	}

	if (FiringType == EFT_Burst || FiringType == EFT_InterruptedBurst)
	{
		if (BurstIterator < BurstAmount - 1) BurstIterator++;
		else { EndShoot(); return; }
	}
	else if (FiringType == EFT_SemiAuto)
	{
		bCanShoot = false;
		FTimerHandle SemiautoHandle;
		GetWorld()->GetTimerManager().SetTimer(
			SemiautoHandle,
			[&]() { bCanShoot = CurrentAmmoAmount > 0 || bInfiniteAmmo; },
			GetWorld()->GetTimerManager().GetTimerRate(ShootHandle),
			false);
	}

	if (ABullet* SpawnedBullet = GetWorld()->SpawnActorDeferred<ABullet>(BulletClass, GetComponentTransform()))
	{
		OnShootFired.AddDynamic(SpawnedBullet, &ABullet::LaunchBullet);
		SpawnedBullet->InitialLifeSpan = BulletLifeTime;
		FVector LaunchVelocity = UKismetMathLibrary::GetForwardVector(GetComponentRotation()) * UKismetMathLibrary::RandomFloatInRange(MuzzleVelocityMin, MuzzleVelocityMax);
		BulletIgnoreActors.Add(SpawnedBullet);

		if (BulletTraceDebugType != EDrawDebugTrace::None)
		{
			// Setting up bullet predict params
			FPredictProjectilePathParams Params;
			Params.StartLocation = GetComponentLocation();
			Params.bTraceWithCollision = bTraceBulletWithCollision;
			Params.MaxSimTime = BulletLifeTime;
			Params.bTraceWithChannel = bTraceBulletWithChannel;
			Params.TraceChannel = BulletTraceChannel;
			Params.ObjectTypes = BulletObjectTypes;
			Params.ActorsToIgnore = BulletIgnoreActors;
			Params.SimFrequency = SimFrequency;
			Params.DrawDebugType = BulletTraceDebugType;
			Params.bTraceComplex = bBulletTraceComplex;
			Params.LaunchVelocity = LaunchVelocity;
			Params.ProjectileRadius = SpawnedBullet->BulletRadius;
			Params.OverrideGravityZ = SpawnedBullet->OverrideBulletGravity;
			Params.DrawDebugTime = GetWorld()->GetTimerManager().GetTimerRate(ShootHandle);

			// Predict bullet path with params
			FPredictProjectilePathResult PredictResult;
			UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult);
		}
		SpawnedBullet->FinishSpawning(GetComponentTransform());
		OnShootFired.Broadcast(LaunchVelocity);
	}

	// Update random fire rate in runtime
	/*float FireRate = UKismetMathLibrary::RandomFloatInRange(FireRateMin, FireRateMax);
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &UBarrel::ShootFired, FireRate, true);*/
}


void UBarrel::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoAmount = InitialAmmoAmount;
}

void UBarrel::EndShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
	ShootHandle.Invalidate();
	BurstIterator = 0;
	BulletIgnoreActors.Empty();
}

void UBarrel::SingleCharge()
{
	if (CurrentAmmoAmount < SpecifiedCharges && CurrentAmmoAmount < MaxAmmoAmount)
	{
		CurrentAmmoAmount++;
		OnSingleCharge.Broadcast(CurrentAmmoAmount);
	}
	else
	{
		int32 rest = SpecifiedCharges - CurrentAmmoAmount;
		SpecifiedCharges = 0;
		GetWorld()->GetTimerManager().ClearTimer(ChargeHandle);
		ChargeHandle.Invalidate();
		OnChargeCompleted.Broadcast(rest);
		bCanShoot = CurrentAmmoAmount > 0;
	}
}
