// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItem.h"
#include "Characters/PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

AInventoryItem::AInventoryItem()
{
	MeshComponent->SetSimulatePhysics(true);
}

void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
}

void AInventoryItem::StartInteraction_Implementation()
{
	if (!CanInteract()) return;
	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(OverlapedActor);
	if (!PlayerRef) return;
	bool bSuccess = PlayerRef->GetInventory()->AddItem(InventorySlot);
	if (bSuccess) UGameplayStatics::PlaySound2D(GetWorld(), PickUpSound);
	if (InventorySlot.Amount == 0) Destroy();
}
