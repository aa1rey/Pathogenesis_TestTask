// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "BaseInteractive.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyToInteractSignature, bool, bReady);

UCLASS()
class PATHOGENESISTESTTASK_API ABaseInteractive : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnReadyToInteractSignature OnReadyToInteract;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actor Components") UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Components") class USphereComponent* OverlapColider;

	UPROPERTY(EditDefaultsOnly) UMaterialInstance* OverlayMaterial;
	bool bOverlayMatSet;
	UPROPERTY(EditDefaultsOnly) FName ActorInteractionTag;
	UPROPERTY(BlueprintReadOnly) AActor* OverlapedActor;
	FTimerHandle CanInteractHandle;
public:	
	ABaseInteractive();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool FromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void StartInteraction_Implementation() override {}
	virtual void HoldInteraction_Implementation() override {}
	virtual void StopInteraction_Implementation() override {}

	UFUNCTION(BlueprintCallable, BlueprintPure) bool CanInteract();
};
