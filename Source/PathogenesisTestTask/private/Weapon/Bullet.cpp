// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "Characters/HealthInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RicochetAmount = 0;
	RicochetIterator = 0;
	Damage = 0.f;

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
	MeshComponent->OnComponentHit.AddDynamic(this, &ABullet::OnImpact);
}

void ABullet::OnImpact_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetClass()->ImplementsInterface(UHealthInterface::StaticClass()))
	{
		IHealthInterface::Execute_SubHealth(OtherActor, Damage);
		UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, DamageCauser, Damage, FVector(), FVector());
	}

	if (RicochetIterator < RicochetAmount) RicochetIterator++;
	else Destroy();
}
