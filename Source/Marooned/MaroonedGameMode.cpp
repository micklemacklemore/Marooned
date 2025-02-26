// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaroonedGameMode.h"
#include "MaroonedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMaroonedGameMode::AMaroonedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
