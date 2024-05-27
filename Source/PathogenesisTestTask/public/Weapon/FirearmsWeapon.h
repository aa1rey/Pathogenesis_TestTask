// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackInterface.h"
#include "Inventory/UseInterface.h"
#include "FirearmsWeapon.generated.h"

UCLASS()
class PATHOGENESISTESTTASK_API AFirearmsWeapon : public AActor, public IAttackInterface, public IUseInterface
{
	GENERATED_BODY()

public:
	AActor* OwnerActor;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USceneComponent* SceneRoot;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USkeletalMeshComponent* MeshComponent;

	// SkeletalMeshComponent is better suited, but StaticMeshComponent will be sufficient for this test task
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") USceneComponent* MuzzleFlashSocket;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") class UBarrel* BarrelComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attachment") FName AttachSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Info") EWeaponType Type;
public:	
	AFirearmsWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void BeginAttack() override;
	virtual void ReleaseAttack() override;
	virtual void Use(class ACharacter* OwnerRef) override;
};
