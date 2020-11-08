// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * A* node
 */
class LOSTVALLEY_API Node
{
public:
	// 0 - white, 1 - gray, 2 - black
	int c = 0;

	int id;

	float distance = std::numeric_limits<float>::max();

	float cost;

	Node* predecssor;

	Node() {}
	Node(int id, float cost) : id(id), cost(cost) {}
	~Node() {}
};