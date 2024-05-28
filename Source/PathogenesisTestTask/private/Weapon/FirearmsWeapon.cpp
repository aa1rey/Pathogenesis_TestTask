// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FirearmsWeapon.h"
#include "Weapon/Barrel.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"

// Sets default values
AFirearmsWeapon::AFirearmsWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AttachSocketName = "ItemSocket";

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(SceneRoot);

	MuzzleFlashSocket = CreateDefaultSubobject<USceneComponent>("Muzzle Flash");
	MuzzleFlashSocket->SetupAttachment(MeshComponent);

	BarrelComponent = CreateDefaultSubobject<UBarrel>("Barrel");
	BarrelComponent->SetupAttachment(MuzzleFlashSocket);
}

// Called when the game starts or when spawned
void AFirearmsWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (BarrelComponent)
	{
		BarrelComponent->OnChargeCompleted.AddDynamic(this, &AFirearmsWeapon::OnChargeCompetedEvent);
		BarrelComponent->OnCurrentAmmoUpdate.AddDynamic(this, &AFirearmsWeapon::OnSingleChargeEvent);
	}
	else UE_LOG(LogTemp, Error, TEXT("BarrelComponent is not valid in object: '%s'!"), *GetNameSafe(this));
}

void AFirearmsWeapon::BeginAttack()
{
	BarrelComponent->Shoot(true);
	BarrelComponent->BulletIgnoreActors.Add(this);
	BarrelComponent->BulletIgnoreActors.Add(OwnerActor);
}

void AFirearmsWeapon::ReleaseAttack()
{
	BarrelComponent->Shoot(false);
}

void AFirearmsWeapon::Reload()
{
	BarrelComponent->Charge(100, false, 1.f);
}

void AFirearmsWeapon::Use(ACharacter* OwnerRef, FInventorySlot Slot)
{
	AttachToComponent(OwnerRef->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachSocketName);
	OwnerActor = OwnerRef;
	Info = Slot.ItemInfo.WeaponInfo;
	Name = Slot.ItemInfo.Name;
	BarrelComponent->SetMaxAmmoAmount(Slot.ItemInfo.WeaponInfo.MaxAmmo);
	BarrelComponent->SetCurrentAmmoAmount(Slot.ItemInfo.WeaponInfo.CurrentAmmo);
}

void AFirearmsWeapon::OnSingleChargeEvent(int32 CurrentAmmoAmount)
{
	Info.CurrentAmmo = CurrentAmmoAmount;
	OnInfoUpdate.Broadcast(Info);
}

void AFirearmsWeapon::OnChargeCompetedEvent(int32 CurrentAmmoAmount, int32 RestedAmmo)
{
	Info.CurrentAmmo = CurrentAmmoAmount;
	OnInfoUpdate.Broadcast(Info);
}
