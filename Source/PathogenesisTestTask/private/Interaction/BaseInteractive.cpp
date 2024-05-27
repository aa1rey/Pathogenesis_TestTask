// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseInteractive.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseInteractive::ABaseInteractive()
{
	PrimaryActorTick.bCanEverTick = false;
	bOverlayMatSet = false;
	ActorInteractionTag = "Interact";
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	

	OverlapColider = CreateDefaultSubobject<USphereComponent>("Overlap Colider");
	OverlapColider->SetupAttachment(MeshComponent);
	OverlapColider->SetSphereRadius(200.f);
	OverlapColider->SetGenerateOverlapEvents(true);	
}

void ABaseInteractive::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapColider->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractive::OnComponentBeginOverlap);
	OverlapColider->OnComponentEndOverlap.AddDynamic(this, &ABaseInteractive::OnComponentEndOverlap);
}

void ABaseInteractive::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(ActorInteractionTag)) return;
	OverlapedActor = OtherActor;
	GetWorldTimerManager().SetTimer(CanInteractHandle, [&]()
		{
			bool bReady = CanInteract();
			OnReadyToInteract.Broadcast(bReady);

			// Loic gates to not to set/unset material every timer tick
			if (bReady && !bOverlayMatSet)
			{
				bOverlayMatSet = true;
				MeshComponent->SetOverlayMaterial(OverlayMaterial);
			}
			else if (!bReady && bOverlayMatSet)
			{
				bOverlayMatSet = false;
				MeshComponent->SetOverlayMaterial(nullptr);
			}
		}, 0.1f, true);
}

void ABaseInteractive::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlapedActor)
	{
		OverlapedActor = nullptr;
		OnReadyToInteract.Broadcast(false);
	}
}

bool ABaseInteractive::CanInteract()
{
	if (!OverlapedActor)
	{
		GetWorldTimerManager().ClearTimer(CanInteractHandle);
		CanInteractHandle.Invalidate();
		return false;
	}

	FHitResult OutHit;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	bool bWasHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(),
		OverlapedActor->GetActorLocation(),
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	return bWasHit && OutHit.GetActor() == OverlapedActor;
}
