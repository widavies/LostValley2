// Fill out your copyright notice in the Description page of Project Settings.


#include "Roadmap.h"
#include <Runtime\Engine\Public\DrawDebugHelpers.h>
#include <Runtime\NavigationSystem\Public\NavigationSystem.h>
#include "Math/UnrealMathUtility.h"
#include <LostValley\Node.h>

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
  mail.Add(0);
  mail.Add(1);
  mail.Add(2);
  mail.Add(3);
  mail.Add(4);
  mail.Add(5);
  mail.Add(6);
  mail.Add(7);
}

bool Roadmap::HasMail() {
  return mail.Num() > 0;
}
int Roadmap::GetMail() {
  int i = mail[0];
  mail.RemoveAt(0);
  return i;
}

Roadmap::~Roadmap() {
}

void Roadmap::GeneratePRM() {
  if(prmBuilt) return;

  for(int i = 0; i < NUM_NODES; i++) {
    for(int j = 0; j < NUM_NODES; j++) {
      neighbors[i][j] = -1;
    }
  }

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
    } else if(AllActorsItr->GetName().Contains(FString("model"))) {
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
  for(int i = 0; i < NUM_GENERATED; i++) {
    // Check to make sure NodeLocation does not intersect with any obstacles
    while(true) {
      float x = FMath::RandRange(start.X, end.X);
      float y = FMath::RandRange(start.Y, end.Y);
      float z = GetMapHeight(FVector2D(x, y));

      FVector NodeLocation(x, y, z);

      bool intersects = false;

      for(int j = 0; j < obstacles.Num(); j++) {
        if(z < SEA_LEVEL || FVector::DistXY(NodeLocation, obstacles[j]) < obstacleRadii[j]) {
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

  // A few handpicked node locations
  for(int i = 0; i < mailboxes.Num(); i++) {
    nodePositions[NUM_GENERATED + i] = mailboxes[i];
  }

  // This is handpicked to ensure that the bridge gets a path
  nodePositions[IX_BRIDGE_1] = FVector(-13320.0f, 38570.0f, 2911.358887f);
  nodePositions[IX_BRIDGE_2] = FVector(-17760.0f, 50530.0f, 2846.772949f);

  nodePositions[IX_POSTBOX] = FVector(-14870.0f, 26030.0f, 3367.158203f);

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

  // Step 2) Connect neighbors to each other
  for(int i = 0; i < NUM_NODES; i++) {
    for(int j = 0; j < NUM_NODES; j++) {
      if(i == j) continue;

      FVector a = nodePositions[i];
      FVector b = nodePositions[j];

      // We need to make sure that this ray is a valid one:
        // It doesn't pass through any objects.
        // It doesn't pass through the lake
      bool intersectsAny = false;
      for(int k = 0; k < obstacles.Num(); k++) {
        if(DropsBelowSeaLevel(a, b) || IntersectsCircle(obstacles[k], obstacleRadii[k], a, b)) {
          intersectsAny = true;
          break;
        }
      }

      if(intersectsAny && !(i == IX_BRIDGE_1 && j == IX_BRIDGE_2) && !(i == IX_BRIDGE_2 && j == IX_BRIDGE_1)) {
        continue;
      }

      // Otherwise, add the connection
      int emptyIndex = -1;
      for(int e = 0; e < NUM_NODES; e++) {
        if(neighbors[i][e] == -1) {
          emptyIndex = e;
          break;
        }
      }

      neighbors[i][emptyIndex] = j;
      cost[i][emptyIndex] = FVector::DistXY(a, b);
    }
  }

  // Debug: Draw connections
  //for(int i = 0; i < NUM_NODES; i++) {
  //  for(int j = 0; j < NUM_NODES; j++) {
  //    if(neighbors[i][j] == -1) {
  //      break;
  //    }

  //    FVector a = nodePositions[i];
  //    FVector b = nodePositions[neighbors[i][j]];
  //    
  //    FVector StartLocation{ a.X, a.Y, a.Z + 200 };
  //    FVector EndLocation{ b.X, b.Y, b.Z + 200 };

  //    DrawDebugLine(
  //      world,
  //      StartLocation,
  //      EndLocation,
  //      FColor::Red,
  //      true,
  //      5.f,
  //      0.f,
  //      10.f);
  //  }
  //}


  prmBuilt = true;
}

bool Roadmap::IntersectsCircle(FVector obstacle, float obstacleRadius, FVector start, FVector end) {
  float x1 = start.X - obstacle.X;
  float x2 = end.X - obstacle.X;
  float y1 = start.Y - obstacle.Y;
  float y2 = end.Y - obstacle.Y;
  float dx = x2 - x1;
  float dy = y2 - y1;
  float dr_squared = dx * dx + dy * dy;
  float D = x1 * y2 - x2 * y1;
  return obstacleRadius * obstacleRadius * dr_squared * 0.2f> D * D;
}

bool Roadmap::DropsBelowSeaLevel(FVector start, FVector end) {
  FVector diff = (end - start);
  FVector frac = diff / 20;

  FVector b = start;

  for(int i = 0; i < 20; i++) {
    if(GetMapHeight(FVector2D(b.X, b.Y)) < SEA_LEVEL) {
      return true;
    }

    b += frac;
  }


  return false;
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

TArray<FVector*> Roadmap::Search(FVector from, int deliveringTo) {

  // Find closest node to 'from'
  TArray<FVector*> path;
  double heuristic[NUM_NODES];
  float minDist = std::numeric_limits<float>::max();
  int start = -1;
  int goal;
  if(deliveringTo == -1) {
    goal = IX_POSTBOX;
  } else {
    goal = NUM_GENERATED + deliveringTo;
  }

  for(int i = 0; i < NUM_NODES; i++) {
    float dist = FVector::DistXY(from, nodePositions[i]);

    if(dist < minDist) {
      minDist = dist;
      start = i;
    }

    heuristic[i] = FVector::DistXY(nodePositions[i], nodePositions[goal]);
  }
  
  // Run A*
  Node* nodes[NUM_NODES];

  for(int i = 0; i < NUM_NODES; i++) {
    nodes[i] = new Node(i, 0);
  }

  auto compare = [](Node* a, Node* b) { return a->cost < b->cost; };
  std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> exploring(compare);

  Node* s = nodes[start];
  s->predecssor = NULL;
  s->distance = 0;
  s->cost = 0;
  s->c = 1;
  
  exploring.push(s);

  while(!exploring.empty()) {
    Node* u = exploring.top();
    exploring.pop();

    if(u->id == goal) {
      Node* n = u;
      while(n != NULL) {
        path.Insert(&nodePositions[n->id], 0);
        n = n->predecssor;
      }
      path.Insert(&nodePositions[start], 0);
      
      for(int i = 0; i < NUM_NODES; i++) delete nodes[i];

      return path;
    }

    for(int j = 0; j < NUM_NODES; j++) {
      int v = neighbors[u->id][j];

      if(v == -1) break;

      Node* vn = nodes[v];
      float dist = cost[u->id][v];

      if(vn->c == 0) {
        vn->c = 1;
        vn->distance = u->distance + dist;
        vn->predecssor = u;
        vn->cost = heuristic[v] + vn->distance;

        exploring.push(vn);
      } else if(vn->cost > u->distance + dist + heuristic[v]) {
        vn->predecssor = u;
        vn->distance = u->distance + dist;
        vn->cost = u->distance + dist + heuristic[v];
        // hack to update priority
        // https://stackoverflow.com/questions/5810190/how-to-tell-a-stdpriority-queue-to-refresh-its-ordering/5810342
        std::make_heap(const_cast<Node**>(&exploring.top()),
          const_cast<Node**>(&exploring.top()) + exploring.size(),
          compare);
      }
      u->c = 2;
    }
  }

  for(int i = 0; i < NUM_NODES; i++) delete nodes[i];
  return TArray<FVector*>();
}