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

	UE_LOG(LogTemp, Warning, TEXT("Making game state..."));
	roadmap = new Roadmap(GetWorld());
}

ALostValleyGameMode::~ALostValleyGameMode() {
	delete roadmap;
}

void ALostValleyGameMode::PostLogin(APlayerController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("Creating PRM..."));
	roadmap->GeneratePRM();
}
