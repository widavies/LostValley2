// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Public\EngineUtils.h>

/**
 * 
 */
class LOSTVALLEY_API Roadmap
{
public:
	Roadmap(UWorld * world);
	~Roadmap();
	FVector WhereTo();
private:
	UWorld* world;
};
