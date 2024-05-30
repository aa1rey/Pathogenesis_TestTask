// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameInstance.h"
#include "SaveGameObject.h"
#include "Kismet/GameplayStatics.h"

void USaveGameInstance::Init()
{
    Super::Init();
}

void USaveGameInstance::Shutdown()
{
    Super::Shutdown();
}

void USaveGameInstance::StartGameInstance()
{
    Super::StartGameInstance();
}

void USaveGameInstance::ClearAllSaves(FString SlotName)
{
    //if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) return;

    UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void USaveGameInstance::SaveLocation(const FString& CharacterName, FVector Location)
{
    if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0)) CreateSaveFile<USaveGameObject>("Slot1");
    USaveGameObject* SaveObject = GetSaveGameObject<USaveGameObject>("Slot1");

    // If TMap contains given CharacterName key - rewrite location, otherwise add new one
    auto* locs = &SaveObject->CharactersLocation;
    if ((*locs).Contains(CharacterName)) (*locs)[CharacterName] = Location;
    else (*locs).Add(CharacterName, Location);

    UGameplayStatics::SaveGameToSlot(SaveObject, "Slot1", 0);
}

bool USaveGameInstance::LoadLocation(const FString& CharacterName, FVector& Location)
{
    if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0)) return false;
    USaveGameObject* SaveObject = GetSaveGameObject<USaveGameObject>("Slot1");

    // If TMap does not contain given CharacterName key - return false, otherwise value is set into Location 
    auto* locs = &SaveObject->CharactersLocation;
    if (!(*locs).Contains(CharacterName)) return false;
    Location = (*locs)[CharacterName];

    return true;
}

void USaveGameInstance::SaveHealth(const FString& CharacterName, float Health)
{
    if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0)) CreateSaveFile<USaveGameObject>("Slot1");
    USaveGameObject* SaveObject = GetSaveGameObject<USaveGameObject>("Slot1");

    // If TMap contains given CharacterName key - rewrite Health, otherwise add new one
    auto* health = &SaveObject->CharactersHealth;
    if ((*health).Contains(CharacterName)) (*health)[CharacterName] = Health;
    else (*health).Add(CharacterName, Health);

    UGameplayStatics::SaveGameToSlot(SaveObject, "Slot1", 0);
}

bool USaveGameInstance::LoadHealth(const FString& CharacterName, float& Health)
{
    if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0)) return false;
    USaveGameObject* SaveObject = GetSaveGameObject<USaveGameObject>("Slot1");

    // If TMap does not contain given CharacterName key - return false, otherwise value is set into Health 
    auto* health = &SaveObject->CharactersHealth;
    if (!(*health).Contains(CharacterName)) return false;
    Health = (*health)[CharacterName];

    return true;
}
