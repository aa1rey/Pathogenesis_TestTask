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
	AActor* DamageCauser;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet") float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet") int32 RicochetAmount;

	int32 RicochetIterator;
public:	
	// Sets default values for this actor's properties
	ABullet();
	UFUNCTION(BlueprintCallable) void LaunchBullet(FVector Velocity);
	void SetBulletDamage(float Value) { Damage = Value; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void OnImpact_Implementation(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
