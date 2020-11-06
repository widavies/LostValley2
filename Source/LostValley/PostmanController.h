// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PostmanController.generated.h"

/**
 * 
 */
UCLASS()
class LOSTVALLEY_API APostmanController : public AAIController
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
public:
	APostmanController();
	void PickedDelay();
};
