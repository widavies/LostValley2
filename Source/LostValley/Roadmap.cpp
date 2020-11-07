// Fill out your copyright notice in the Description page of Project Settings.


#include "Roadmap.h"


// Defines the PRM for the map

// TODO
  // Need bounding area to generate points within
  // Need landscape heights to generate points at?
  // Need obstacles: lake, trees, houses, mailboxes

Roadmap::Roadmap(UWorld * world) : world(world)
{
  

}

Roadmap::~Roadmap()
{
}

FVector Roadmap::WhereTo(FVector currentLocation, FString goal) {
  FActorIterator AllActorsItr = FActorIterator(world);

  //While not reached end (overloaded bool operator)
  while(AllActorsItr) {
    if(AllActorsItr->GetName().Equals(FString("tree1_Anim19"))) {
      return AllActorsItr->GetActorLocation();
      break;
    }

    //next actor
    ++AllActorsItr;
  }

  return FVector();
}
