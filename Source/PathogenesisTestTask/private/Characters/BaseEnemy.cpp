// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseEnemy.h"
#include "Characters/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/AttackInterface.h"
#include "Weapon/ReloadInterface.h"
#include "Weapon/Barrel.h"
#include "SaveGameInstance.h"

using enum EAttachmentRule;

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	AttachWeaponSocketName = "WeaponSocket";
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &ABaseEnemy::OnDeath);
	else UE_LOG(LogTemp, Error, TEXT("Health Component is not valid for object: '%s'"), *GetNameSafe(this));

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		WeaponRef = GetWorld()->SpawnActor<IAttackInterface>(WeaponClass, SpawnParams);
		if (!WeaponRef) return;

		Cast<AActor>(WeaponRef)->AttachToComponent(
			/*AttachWeaponSocketName*/FindComponentByTag<USceneComponent>("WeaponAttachment"),
			FAttachmentTransformRules(SnapToTarget, true)/*, AttachWeaponSocketName*/);

		if (IReloadInterface* ReloadInterface = Cast<IReloadInterface>(WeaponRef))
		{
			ReloadInterface->GetBarrelComponent()->bInfiniteAmmo = true;
		}
	}
	else UE_LOG(LogTemp, Log, TEXT("'%s' has no WeaponClass. Object will be run without it."), *GetNameSafe(this));
}

void ABaseEnemy::AddHealth_Implementation(float Value)
{
	if (HealthComponent) HealthComponent->AddHealth(Value);
}

void ABaseEnemy::SubHealth_Implementation(float Value)
{
	if (HealthComponent) HealthComponent->SubHealth(Value);
}

void ABaseEnemy::OnDeath()
{
	if (WeaponRef) Cast<AActor>(WeaponRef)->Destroy();
	Destroy();
}

void ABaseEnemy::BeginAttack()
{
	if (WeaponRef) WeaponRef->BeginAttack();
}

void ABaseEnemy::EndAttack()
{
	if (WeaponRef) WeaponRef->ReleaseAttack();
}

bool ABaseEnemy::Save_Implementation()
{
	if (USaveGameInstance* GameInstance = GetGameInstance<USaveGameInstance>())
	{
		GameInstance->SaveHealth(GetName(), HealthComponent->GetCurrentHealth());
		GameInstance->SaveLocation(GetName(), GetActorLocation());
		return true;
	}
	else return false;
}

bool ABaseEnemy::Load_Implementation()
{
	if (USaveGameInstance* GameInstance = GetGameInstance<USaveGameInstance>())
	{
		float LoadedHealth;
		bool SuccessHealth = GameInstance->LoadHealth(GetName(), LoadedHealth);
		if (SuccessHealth) HealthComponent->SetCurrentHealth(LoadedHealth);

		FVector LoadedLocation;
		bool SuccessLocation = GameInstance->LoadLocation(GetName(), LoadedLocation);
		if (SuccessLocation) SetActorLocation(LoadedLocation);
		return SuccessLocation && SuccessHealth;
	}
	else return false;
}