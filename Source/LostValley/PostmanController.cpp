// Fill out your copyright notice in the Description page of Project Settings.


#include "PostmanController.h"
#include "NavigationSystem.h"


APostmanController::APostmanController() {

}

FVector goTo;

// https://community.gamedev.tv/t/random-location/146210/8
void APostmanController::BeginPlay() {
  Super::BeginPlay();

  
  UE_LOG(LogTemp, Warning, TEXT("I have somewhere to go2."));

  if(GetWorld()) {
    //auto gamemode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();

    //FVector goal = gamemode->roadmap->WhereTo(FVector(0.0f, 0.0f, 0.0f), FString("goal"));

    UE_LOG(LogTemp, Log, TEXT("Got world. Now trying to find nav area for random loc..."));
    // get the current nav system from the world
    //UNavigationSystem* navSystem = UNavigationSystem::GetCurrent(GetWorld());
    // check that we have a nav system
    UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

    if(NavigationArea) {
      UE_LOG(LogTemp, Log, TEXT("Found nav area"));
      // get our actors current location to pe used as our start position
      FVector startPosi = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
      // create a nav location to be used as our end position 
      // default will be set to our current position in case of failure to find a suitable end position
      FNavLocation endPosi;

      UE_LOG(LogTemp, Error, TEXT("Test navArea: %s"), *NavigationArea->GetPathName());

      // attempt to get a random new position
      if(NavigationArea->GetRandomReachablePointInRadius(startPosi, 50, endPosi)) {
        // if we were successfull in finding a new location...
        UE_LOG(LogTemp, Log, TEXT("Found new random loc"));
        goTo = endPosi.Location;
        FTimerHandle UnusedHandle;
        GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APostmanController::PickedDelay, 2.0f, false);
      } else {
        UE_LOG(LogTemp, Error, TEXT("Random loc failed!"));
      }
    }
  }
}

void APostmanController::PickedDelay() {
  EPathFollowingRequestResult::Type type = MoveToLocation(goTo);
  if(type == EPathFollowingRequestResult::Failed) {
    UE_LOG(LogTemp, Error, TEXT("REE!"));
  } else {
    UE_LOG(LogTemp, Error, TEXT("WE got em"));
  }
}