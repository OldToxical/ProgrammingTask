// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProgrammingTaskGameMode.h"
#include "ProgrammingTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProgrammingTaskGameMode::AProgrammingTaskGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
