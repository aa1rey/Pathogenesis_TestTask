// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FirearmsWeapon.h"
#include "Weapon/Barrel.h"
#include "GameFramework/Character.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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
		BarrelComponent->OnShootFired.AddDynamic(this, &AFirearmsWeapon::OnShootFired);
		BarrelComponent->OnOutOfAmmo.AddDynamic(this, &AFirearmsWeapon::OnOutOfAmmo);
	}
	else UE_LOG(LogTemp, Error, TEXT("BarrelComponent is not valid in object: '%s'!"), *GetNameSafe(this));
}

void AFirearmsWeapon::BeginAttack()
{
	BarrelComponent->Shoot(true);
	BarrelComponent->BulletIgnoreActors.Add(this);
	if (OwnerActor) BarrelComponent->BulletIgnoreActors.Add(OwnerActor);
}

void AFirearmsWeapon::ReleaseAttack()
{
	BarrelComponent->Shoot(false);
}

void AFirearmsWeapon::Reload(int32 ReloadAmmoAmount)
{
	BarrelComponent->Charge(ReloadAmmoAmount);
}

void AFirearmsWeapon::Use_Implementation(ACharacter* OwnerRef, FInventorySlot Slot)
{
	AttachToComponent(OwnerRef->FindComponentByTag<USceneComponent>("WeaponAttachment"), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	
	//AttachToComponent(OwnerRef->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachSocketName);
	OwnerActor = OwnerRef;
	BarrelComponent->DamageCauser = OwnerActor;

	Info = Slot.ItemInfo.WeaponInfo;
	Name = Slot.ItemInfo.Name;

	BarrelComponent->SetMaxAmmoAmount(Info.MaxAmmo);
	BarrelComponent->SetCurrentAmmoAmount(Info.CurrentAmmo);
}

void AFirearmsWeapon::OnSingleChargeEvent(int32 CurrentAmmoAmount)
{
	Info.CurrentAmmo = CurrentAmmoAmount;
	OnInfoUpdate.Broadcast(Info);
}

void AFirearmsWeapon::OnChargeCompetedEvent(int32 CurrentAmmoAmount, int32 RestedAmmo)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, BarrelComponent->GetComponentLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
	Info.CurrentAmmo = CurrentAmmoAmount;
	OnInfoUpdate.Broadcast(Info);
}

void AFirearmsWeapon::OnShootFired(FVector Velocity)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, BarrelComponent->GetComponentLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
}

void AFirearmsWeapon::OnOutOfAmmo()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutOfAmmoSound, BarrelComponent->GetComponentLocation(), 1.f, 1.f, 0.f, SoundAttenuation);
}
