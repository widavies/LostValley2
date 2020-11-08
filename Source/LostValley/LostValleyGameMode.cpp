// Copyright Epic Games, Inc. All Rights Reserved.

#include "LostValleyGameMode.h"
#include "LostValleyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALostValleyGameMode::ALostValleyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/PersonBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	roadmap = new Roadmap(GetWorld());
}

void ALostValleyGameMode::PostLogin(APlayerController* NewPlayer) {
	roadmap->GeneratePRM();
}
