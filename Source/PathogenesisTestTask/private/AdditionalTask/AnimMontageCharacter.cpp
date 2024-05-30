// Fill out your copyright notice in the Description page of Project Settings.


#include "AdditionalTask/AnimMontageCharacter.h"
#include "PlayMontageCallbackProxy.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AAnimMontageCharacter::AAnimMontageCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bCanMove = true;
}

void AAnimMontageCharacter::AsyncLoadAnimMontage(int32 Index)
{
	if (!ArrayOfMontages.IsValidIndex(Index)) return;

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, *FString::Printf(TEXT("Before Request | Is loaded: %i"), ArrayOfMontages[Index].IsValid()));

	FStreamableManager& StreamManager = UAssetManager::Get().GetStreamableManager();

	// Create a delegate function that will be called after asynchronous asset loading
	FStreamableDelegate StreamDelegate;
	StreamDelegate.BindUObject(this, &AAnimMontageCharacter::OnAsyncLoadCompleted, Index);

	// Create a request to asynchronously load an element of array, and assign a callback
	TSharedPtr<FStreamableHandle> StreamHandle = StreamManager.RequestAsyncLoad(ArrayOfMontages[Index].ToSoftObjectPath(), StreamDelegate);
}

void AAnimMontageCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AAnimMontageCharacter::OnAsyncLoadCompleted(int32 Index)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Red, *FString::Printf(TEXT("After Request | Is loaded: %i"), ArrayOfMontages[Index].IsValid()));
	PlayAnim(ArrayOfMontages[Index].Get());
}

void AAnimMontageCharacter::MontageCompleted(FName NotifyName)
{
	bCanMove = true;
	PlayMontageCallbackProxy->OnCompleted.RemoveDynamic(this, &AAnimMontageCharacter::MontageCompleted);
	PlayMontageCallbackProxy->OnInterrupted.RemoveDynamic(this, &AAnimMontageCharacter::MontageCompleted);
	PlayMontageCallbackProxy->OnNotifyBegin.RemoveDynamic(this, &AAnimMontageCharacter::NotifyBegin);
	PlayMontageCallbackProxy->OnNotifyEnd.RemoveDynamic(this, &AAnimMontageCharacter::NotifyEnd);
}

void AAnimMontageCharacter::NotifyBegin(FName NotifyName)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, *FString::Printf(TEXT("NotifyBegin! NotifyName: %s"), *NotifyName.ToString()));
}

void AAnimMontageCharacter::NotifyEnd(FName NotifyName)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, *FString::Printf(TEXT("NotifyEnd! NotifyName: %s"), *NotifyName.ToString()));
}

void AAnimMontageCharacter::PlayAnim(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay || !bCanMove) return;
	bCanMove = false;
	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), MontageToPlay);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &AAnimMontageCharacter::MontageCompleted);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &AAnimMontageCharacter::MontageCompleted);
	PlayMontageCallbackProxy->OnNotifyBegin.AddDynamic(this, &AAnimMontageCharacter::NotifyBegin);
	PlayMontageCallbackProxy->OnNotifyEnd.AddDynamic(this, &AAnimMontageCharacter::NotifyEnd);
}