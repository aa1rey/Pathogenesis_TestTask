// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PATHOGENESISTESTTASK_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly) float BulletRadius;
	UPROPERTY(EditDefaultsOnly) float OverrideBulletGravity;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UStaticMeshComponent* MeshComponent;
public:	
	// Sets default values for this actor's properties
	ABullet();
	UFUNCTION(BlueprintCallable) void LaunchBullet(FVector Velocity);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
