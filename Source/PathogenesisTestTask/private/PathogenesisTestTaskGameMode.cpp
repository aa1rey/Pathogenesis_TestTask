// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathogenesisTestTaskGameMode.h"
#include "PathogenesisTestTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"

APathogenesisTestTaskGameMode::APathogenesisTestTaskGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
