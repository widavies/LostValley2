// Fill out your copyright notice in the Description page of Project Settings.


#include "Roadmap.h"


// Defines the PRM for the map

// TODO
  // Need bounding area to generate points within
  // Need landscape heights to generate points at?
  // Need obstacles: lake, trees, houses, mailboxes

Roadmap::Roadmap(UWorld * world) : world(world)
{
  //world->GetNavigationSystem()->
  //UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
  //FBox map = NavigationArea->GetNavigationBounds().Array()[0].AreaBox;
  //FVector center = map.GetCenter() - map.GetSize() / 2;
}

Roadmap::~Roadmap()
{
}

FVector Roadmap::WhereTo() {
  FActorIterator AllActorsItr = FActorIterator(world);

  //While not reached end (overloaded bool operator)
  while(AllActorsItr) {
    if(AllActorsItr->GetName().Equals(FString("tree1_Anim19"))) {
      return AllActorsItr->GetActorLocation();
    }

    //next actor
    ++AllActorsItr;
  }

  return FVector();
}
