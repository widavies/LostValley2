// Fill out your copyright notice in the Description page of Project Settings.


#include "PostmanController.h"
#include "NavigationSystem.h"
#include "LostValleyGameMode.h"

APostmanController::APostmanController() {

}

TArray<FVector*> instructions;
int dest = -1;
int at = -1;
bool ready = false;
FVector Location(-15762.257812f, 27374.837891f, 3511.42041f);

// https://community.gamedev.tv/t/random-location/146210/8
void APostmanController::BeginPlay() {
  Super::BeginPlay();

  if(GetWorld()) {
    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APostmanController::PickedDelay, 2.0f, false);
  }
}

void APostmanController::PickedDelay() {
  ALostValleyGameMode* GameMode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();
  instructions = GameMode->roadmap->Search(Location, dest);
  at = 0;
  NextPath();
}

void APostmanController::NextPath() {
  if(at != -1 && at < instructions.Num()) {
    FVector goal = *instructions[at++];
    Location = goal;

    UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    FNavLocation destination;
    NavigationArea->GetRandomReachablePointInRadius(goal, 500.f, destination);

    EPathFollowingRequestResult::Type type = MoveToLocation(destination.Location, 500.f, true, false);
    if(type == EPathFollowingRequestResult::Failed) {
      UE_LOG(LogTemp, Error, TEXT("Error."));
    } else {
      UE_LOG(LogTemp, Error, TEXT("Following path..."));
    }
  } 
  
  if(at == instructions.Num()) {
    UE_LOG(LogTemp, Error, TEXT("Got assignment"));

    ALostValleyGameMode* GameMode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();

    if(GameMode->roadmap->HasMail()) {
      if(dest == -1) {
        dest = GameMode->roadmap->GetMail();
      } else {
        dest = -1;
      }
      UE_LOG(LogTemp, Error, TEXT("Got assignment: %i"), dest);
      FTimerHandle UnusedHandle;
      GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APostmanController::PickedDelay, 2.0f, false);
    } else {
      GetWorld()->DestroyActor(this);
    }
  }
}

void APostmanController::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void APostmanController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) {
  UE_LOG(LogTemp, Error, TEXT("Move completed!"));
  NextPath();
}