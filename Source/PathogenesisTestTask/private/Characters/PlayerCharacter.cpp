// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/UseInterface.h"
#include "Weapon/AttackInterface.h"
#include "Weapon/ReloadInterface.h"
#include "Characters/EquipmentComponent.h"
#include "Weapon/Barrel.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Tags.Add("Interact");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	EquipmentSlotsAmount = 2;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	TargetArmScrollStep = -10.f;
	MaxTargetArmLength = 1000.f;
	MinTargetArmLength = 400.f;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = MaxTargetArmLength;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	InventoryComponent->SlotsAmount = 15;
	InventoryComponent->SlotsPerRow = 5;

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("Equipment");
	EquipmentComponent->MaxAmount = EquipmentSlotsAmount;

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Scrolling Camera
		EnhancedInputComponent->BindAction(ScrollCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ScrollCamera);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Canceled, this, &APlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::HoldInteract);

		// Open Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &APlayerCharacter::Inventory);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::BeginAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseAttack);

		//Select Weapon
		EnhancedInputComponent->BindAction<APlayerCharacter, int32>(SelectPrimaryWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SelectWeapon, 0);
		EnhancedInputComponent->BindAction<APlayerCharacter, int32>(SelectSecondaryWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SelectWeapon, 1);

		// Reload Weapon
		EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReloadWeapon);
	}
	else UE_LOG(LogTemp, Error, TEXT("Error! Can not get Enhanced Input Component in Player Character"));
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Bind delegates from inventory component
	if (InventoryComponent)
	{
		InventoryComponent->OnItemUse.AddDynamic(this, &APlayerCharacter::OnItemUse);
	}
	else UE_LOG(LogInventory, Error, TEXT("Could not create InventoryComponent for object: '%s'"), *GetNameSafe(this));
}

void APlayerCharacter::OnItemUse(FInventorySlot Slot)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IUseInterface* UsingItem = GetWorld()->SpawnActor<IUseInterface>(Slot.ItemClass, GetMesh()->GetComponentTransform(), SpawnParams))
	{
		if (IAttackInterface* local_WeaponRef = Cast<IAttackInterface>(UsingItem))
		{
			// If new weapon implements this interface - Bind event that would be called every time Current Amount Update
			if (IReloadInterface* new_firearms_weapon = Cast<IReloadInterface>(local_WeaponRef))
				new_firearms_weapon->GetBarrelComponent()->OnCurrentAmmoUpdate.AddDynamic(this, &APlayerCharacter::OnWeaponCurrentAmountUpdateEvent);

			// If equiped weapon exists - destroy it
			if (WeaponRef)
			{
				// If current weapon implements this interface - Unbind Current Amount Update Event
				if (IReloadInterface* curr_firearms_weapon = Cast<IReloadInterface>(WeaponRef))
					curr_firearms_weapon->GetBarrelComponent()->OnCurrentAmmoUpdate.RemoveDynamic(this, &APlayerCharacter::OnWeaponCurrentAmountUpdateEvent);
				
				// Update old inventory slot weapon info
				FInventorySlot slot; int32 found_index;
				if (InventoryComponent->SearchItemByClass(WeaponRef->_getUObject()->GetClass(), slot, found_index))
				{
					slot.ItemInfo.WeaponInfo = WeaponRef->GetWeaponInfo();
					InventoryComponent->UpdateSlotAtIndex(found_index, slot);
				}

				Cast<AActor>(WeaponRef)->Destroy();
			}

			// Set new weapon to equiped
			WeaponRef = local_WeaponRef;
			OnWeaponSwitch.Broadcast(Slot.ItemInfo.WeaponInfo.Type);
		}
		UsingItem->Use(this, Slot);
	}
}

void APlayerCharacter::OnWeaponCurrentAmountUpdateEvent(int32 Amount)
{
	FWeaponInfo WeaponInfo = Cast<IAttackInterface>(WeaponRef)->GetWeaponInfo();
	OnWeaponInfoUpdate.Broadcast(WeaponInfo);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	FVector2D MovementVector = Value.Get<FVector2D>();
	// Find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (LookAxisVector.X != 0 && LookAxisVector.Y != 0)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::ScrollCamera(const FInputActionValue& Value)
{
	float ScrollValue = TargetArmScrollStep * Value.Get<float>();

	float* TAL = &SpringArmComponent->TargetArmLength;
	if (*TAL + ScrollValue >= MinTargetArmLength && *TAL + ScrollValue <= MaxTargetArmLength)
		*TAL += ScrollValue;
	else *TAL = *TAL;
}

bool APlayerCharacter::CanInteract()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* actor : OverlappingActors)
	{
		bool IsImplement = actor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass());
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, *FString::Printf(TEXT("Interation Intrface: %i"), IsImplement));
		if (IsImplement) { InteractedObject = actor; return true; }
	}
	return false;
}

void APlayerCharacter::Interact()
{
	if (CanInteract()) IInteractionInterface::Execute_StartInteraction(InteractedObject);
}

void APlayerCharacter::HoldInteract()
{
	if (Controller && CanInteract()) IInteractionInterface::Execute_HoldInteraction(InteractedObject);
}

void APlayerCharacter::Inventory()
{
	InventoryComponent->ToggleInventoryVisibility();
}

void APlayerCharacter::BeginAttack()
{
	if (WeaponRef) WeaponRef->BeginAttack();
}

void APlayerCharacter::ReleaseAttack()
{
	if (WeaponRef) WeaponRef->ReleaseAttack();
}

void APlayerCharacter::SelectWeapon(int32 Index)
{
	// Get inventory index from equipment component
	FInventorySlot slot;
	int32 found_index = EquipmentComponent->GetContentAtIndex(Index);

	// Get inventory slot info by found index
	if (InventoryComponent->GetItemInfoAtIndex(found_index, slot))
	{
		FString NewName = slot.ItemClass->GetName();
		if (WeaponRef)
		{
			// Get inventory slot of currently equiped weapon
			FInventorySlot slot_curr; int32 flound_index_curr;
			if (InventoryComponent->SearchItemByClass(WeaponRef->_getUObject()->GetClass(), slot_curr, flound_index_curr))
			{
				OnWeaponSelect.Broadcast((int32)slot_curr.ItemInfo.WeaponInfo.Priority - 1, false);
				// If weapon exists AND it's the same that incomming, unequip and destroy
				FString CurrName = WeaponRef->_getUObject()->GetClass()->GetName();
				if (NewName == CurrName)
				{
					// Update old inventory slot weapon info
					slot_curr.ItemInfo.WeaponInfo = WeaponRef->GetWeaponInfo();
					InventoryComponent->UpdateSlotAtIndex(flound_index_curr, slot_curr);

					Cast<AActor>(WeaponRef)->Destroy();
					WeaponRef = nullptr;
					OnWeaponSwitch.Broadcast(EWeaponType::EWT_None);
					return;
				}
			}
		}
		OnWeaponSelect.Broadcast(Index, true);
		InventoryComponent->UseItemAtIndex(found_index);
	}
}

void APlayerCharacter::ReloadWeapon()
{
	if (WeaponRef)
		if (WeaponRef->_getUObject()->GetClass()->ImplementsInterface(UReloadInterface::StaticClass()))
			Cast<IReloadInterface>(WeaponRef)->Reload();
}