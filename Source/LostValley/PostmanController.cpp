// Fill out your copyright notice in the Description page of Project Settings.


#include "PostmanController.h"
#include "NavigationSystem.h"
#include "LostValleyGameMode.h"

APostmanController::APostmanController() {

}

TArray<FVector*> instructions;
int at = -1;
bool ready = false;
FVector Location(-15762.257812f, 27374.837891f, 3511.42041f);

// https://community.gamedev.tv/t/random-location/146210/8
void APostmanController::BeginPlay() {
  Super::BeginPlay();

  if(GetWorld()) {
    ALostValleyGameMode* GameMode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();
    GameMode->roadmap->GeneratePRM();
    instructions = GameMode->roadmap->Search(Location, 0);
    at = 0;

    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APostmanController::PickedDelay, 2.0f, false);
  }
}

void APostmanController::PickedDelay() {
  NextPath();
}

void APostmanController::NextPath() {
  if(at != -1 && at < instructions.Num()) {
    FVector goal = *instructions[at++];
    Location = goal;

    UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    FNavLocation destination;
    NavigationArea->GetRandomReachablePointInRadius(goal, 8000, destination);

    EPathFollowingRequestResult::Type type = MoveToLocation(destination.Location);
    if(type == EPathFollowingRequestResult::Failed) {
      UE_LOG(LogTemp, Error, TEXT("REE!"));
    } else {
      UE_LOG(LogTemp, Error, TEXT("Following path..."));
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