// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimMontageCharacter.generated.h"

UCLASS()
class PATHOGENESISTESTTASK_API AAnimMontageCharacter : public ACharacter
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY(BlueprintReadWrite) bool bCanMove;

	// Array storing soft pointers to Animation Montages that will not be loaded into memory until they are needed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<TSoftObjectPtr<UAnimMontage>> ArrayOfMontages;

	// Anim montage callback
	class UPlayMontageCallbackProxy* PlayMontageCallbackProxy;
public:
	AAnimMontageCharacter();

	// Asynchronously load an anim montage from ArrayOfMontages by given index
	UFUNCTION(BlueprintCallable) void AsyncLoadAnimMontage(int32 Index);

	// Run specified anim montage
	void PlayAnim(UAnimMontage* MontageToPlay);
protected:
	virtual void BeginPlay() override;

	// After the async load is completed, is called and runs PlayAnim function
	void OnAsyncLoadCompleted(int32 Index);

	// Is called when anim montage completed or interrupted
	UFUNCTION() void MontageCompleted(FName NotifyName);

	// Is called when anim montage sequence enters any notify
	UFUNCTION() void NotifyBegin(FName NotifyName);

	// Is called when anim montage sequence exits any notify
	UFUNCTION() void NotifyEnd(FName NotifyName);
};
