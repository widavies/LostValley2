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
	// Runs A* on the PRM, where from the agent's current location, and deliverTo is the intended destination
	TArray<FVector*> Search(FVector from, int deliveringTo);
	// Gets the Z coordinate at the specified point
	float GetMapHeight(FVector2D Point);
	// Generates the PRM
	void GeneratePRM();
	// Checks if the line defined by start&end intersects with circle defined by obstacle&radius
	bool IntersectsCircle(FVector obstacle, float obstacleRadius, FVector start, FVector end);
	// Checks if any point along the line defined by start & end drops below sea level
	bool DropsBelowSeaLevel(FVector start, FVector end);
	// Checks if the postbox has mail
	bool HasMail();
	// Gets a letter to deliver from the postbox
	int GetMail();
	// Adds a letter to the postbox
	void AddMail(int address);
	// The node locations
	FVector nodePositions[NUM_NODES];
	// The distance between nodes
	int neighbors[NUM_NODES][NUM_NODES];
	// Costs between nodes (only XY, Z is ignored)
	float cost[NUM_NODES][NUM_NODES];
	// Whether the prm has already been built or not
	bool prmBuilt = false;
	int waitingMail = 10;
	TArray<int> mail;
	UWorld* world;
};
