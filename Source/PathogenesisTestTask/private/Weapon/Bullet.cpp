// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	MeshComponent->SetSimulatePhysics(true);

}

void ABullet::LaunchBullet(FVector Velocity)
{
	MeshComponent->SetPhysicsLinearVelocity(Velocity);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}