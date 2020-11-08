// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Public\EngineUtils.h>

/**
 * 
 */
const int NUM_NODES = 1000;

class LOSTVALLEY_API Roadmap
{
public:
	Roadmap(UWorld * world);
	~Roadmap();
	FVector WhereTo();
	float GetMapHeight(FVector2D Point);
	void GeneratePRM();
	FVector nodePositions[NUM_NODES];
	UWorld* world;
};
