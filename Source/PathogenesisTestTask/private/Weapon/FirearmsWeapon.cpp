// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FirearmsWeapon.h"
#include "Weapon/Barrel.h"
#include "GameFramework/Character.h"

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
	WeaponType = Type;
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

void AFirearmsWeapon::Use(ACharacter* OwnerRef)
{
	AttachToComponent(OwnerRef->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachSocketName);
	OwnerActor = OwnerRef;
}
