// Fill out your copyright notice in the Description page of Project Settings.


#include "Roadmap.h"
#include <Runtime\Engine\Public\DrawDebugHelpers.h>
#include <Runtime\NavigationSystem\Public\NavigationSystem.h>
#include "Math/UnrealMathUtility.h"


// Defines the PRM for the map

// TODO
  // Need bounding area to generate points within
  // Need landscape heights to generate points at?
  // Need obstacles: lake, trees, houses, mailboxes

Roadmap::Roadmap(UWorld* world) : world(world) {
  //world->GetNavigationSystem()->
  //UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
  //FBox map = NavigationArea->GetNavigationBounds().Array()[0].AreaBox;
  //FVector center = map.GetCenter() - map.GetSize() / 2;
}

Roadmap::~Roadmap() {
}

void Roadmap::GeneratePRM() {
  // Step 0) Get obstacle (trees, houses) locations, mailboxes (destinations), postbox (starting)
  FVector postbox;
  TArray<FVector> mailboxes;

  TArray<FVector> obstacles;
  TArray<float> obstacleRadii;
  
  //While not reached end (overloaded bool operator)
  FActorIterator AllActorsItr = FActorIterator(world);
  while(AllActorsItr) {
    float radius;
    float halfHeight;
    AllActorsItr->GetSimpleCollisionCylinder(radius, halfHeight);

    if(AllActorsItr->GetName().Contains(FString("Mailbox"))) {
      mailboxes.Add(AllActorsItr->GetActorLocation());
    } else if(AllActorsItr->GetName().Contains(FString("tree"))) {
      obstacles.Add(AllActorsItr->GetActorLocation());
      obstacleRadii.Add(radius); // radius of tree
    } else if(AllActorsItr->GetName().Contains(FString("house"))) {
      obstacles.Add(AllActorsItr->GetActorLocation());
      obstacleRadii.Add(radius); // radius of house
    } else if(AllActorsItr->GetName().Equals(FString("PostBox"))) {
      postbox = AllActorsItr->GetActorLocation();
    }

    //next actor
    ++AllActorsItr;
  }

  // Note: This project does not actually use the navigation system. It only uses it for
  // a 3D box defining the valid map space, as the actual map is a bit bigger than is needed. The
  // only thing used from the navigation area is the x,y coordinates and size of the available map
  // area
  UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
  FBox map = NavigationArea->GetNavigationBounds().Array()[0].AreaBox;
  FVector start = map.GetCenter() - map.GetSize() / 2;
  FVector end = map.GetCenter() + map.GetSize() / 2;

  // Step 1) Generate N random points within the navigation bounds
  for(int i = 0; i < NUM_NODES; i++) {
    // Check to make sure NodeLocation does not intersect with any obstacles
    while(true) {
      float x = FMath::RandRange(start.X, end.X);
      float y = FMath::RandRange(start.Y, end.Y);
      float z = GetMapHeight(FVector2D(x, y));

      FVector NodeLocation(x, y, z);

      bool intersects = false;

      for(int j = 0; j < obstacles.Num(); j++) {
        if(FVector::DistXY(NodeLocation, obstacles[j]) < obstacleRadii[j]) {
          intersects = true;
          break;
        }
      }

      if(intersects) continue;
      else {
        nodePositions[i] = NodeLocation;
        break;
      }
    }
  }

  // Debug: Draw node locations
  //for(int i = 0; i < NUM_NODES; i++) {
  //  float x = nodePositions[i].X;
  //  float y = nodePositions[i].Y;

  //  FVector StartLocation{ x, y, 10000 };
  //  FVector EndLocation{ x, y, -100 };

  //  DrawDebugLine(
  //    world,
  //    StartLocation,
  //    EndLocation,
  //    FColor::Red,
  //    true,
  //    5.f,
  //    0.f,
  //    10.f
  //  );
  //}


  // Generate N random points within the navigation bounds
    // Set their Z coordinate using GetMapHeight
    // If the location is too close to an obstacle, or its z is below sea level, discard it.
  // Attempt to connect points
    // Rays should not intersect any obstacles
    // Rays shouldn't dip below sea level
  // Also record cost of Ray (distance)
  // Node/Edge structure is then setup.
  // WhereTo should take a few pieces of information:
    // What is the address of your mail?
    // Otherwise post box
    // Return next node to go to.
}

float Roadmap::GetMapHeight(FVector2D Point) {
  if(world) {
    FVector StartLocation{ Point.X, Point.Y, 10000 };    // Raytrace starting point.
    FVector EndLocation{ Point.X, Point.Y, -100 };            // Raytrace end point.

    // Raytrace for overlapping actors.
    FHitResult HitResult;
    world->LineTraceSingleByObjectType(
      OUT HitResult,
      StartLocation,
      EndLocation,
      FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
      FCollisionQueryParams()
    );

    // Return Z location.
    if(HitResult.GetActor()) return HitResult.ImpactPoint.Z;
  }

  return 0;
}

FVector Roadmap::WhereTo() {
  FActorIterator AllActorsItr = FActorIterator(world);

  float h = GetMapHeight(FVector2D(-7920.0f, 20480.0f));

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
