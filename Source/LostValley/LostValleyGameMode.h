// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <LostValley\Roadmap.h>

#include "LostValleyGameMode.generated.h"

UCLASS(minimalapi)
class ALostValleyGameMode : public AGameModeBase {
  GENERATED_BODY()


public:
  ALostValleyGameMode();
  Roadmap* roadmap;
  virtual void PostLogin(APlayerController* NewPlayer) override;
};



