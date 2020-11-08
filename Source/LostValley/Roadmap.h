// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Public\EngineUtils.h>

/**
 * 
 */
const int NUM_GENERATED = 300;
const int NUM_NODES = NUM_GENERATED + 11;
const int IX_POSTBOX = NUM_NODES - 1;
const int IX_BRIDGE_1 = NUM_NODES - 2;
const int IX_BRIDGE_2 = NUM_NODES - 3;

const float SEA_LEVEL = 1350.0f;

class LOSTVALLEY_API Roadmap
{
public:
	Roadmap(UWorld * world);
	~Roadmap();
	// deliveringTo == -1 if has no mail
	TArray<FVector*> Search(FVector from, int deliveringTo);
	float GetMapHeight(FVector2D Point);
	void GeneratePRM();
	bool IntersectsCircle(FVector obstacle, float obstacleRadius, FVector start, FVector end);
	bool DropsBelowSeaLevel(FVector start, FVector end);
	bool HasMail();
	int GetMail();
	void AddMail(int address);
	FVector nodePositions[NUM_NODES];
	// The distance between nodes
	int neighbors[NUM_NODES][NUM_NODES];
	float cost[NUM_NODES][NUM_NODES];
	bool prmBuilt = false;
	int waitingMail = 10;
	TArray<int> mail;
	UWorld* world;
};
