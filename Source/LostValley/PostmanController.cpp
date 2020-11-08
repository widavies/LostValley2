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
APawn* pawn;

// https://community.gamedev.tv/t/random-location/146210/8
void APostmanController::BeginPlay() {
  Super::BeginPlay();

  if(GetWorld()) {
    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APostmanController::PickedDelay, 2.0f, false);
  }
}

void APostmanController::PickedDelay() {
  UE_LOG(LogTemp, Warning, TEXT("Delivering mail to %i"), dest);
}

void APostmanController::NextPath() {
  if(at != -1 && at < instructions.Num()) {
    UE_LOG(LogTemp, Warning, TEXT("Performing step %i / %i"), at, instructions.Num() - 1);
    FVector goal = *instructions[at++];
    
    UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    FNavLocation destination;
    NavigationArea->GetRandomReachablePointInRadius(goal, 300.f, destination);

    EPathFollowingRequestResult::Type type = MoveToLocation(destination.Location, 300.f, true, false, false, false);
    if(type == EPathFollowingRequestResult::Failed) {
      UE_LOG(LogTemp, Error, TEXT("Error."));
    }
  } 
  else if(at == instructions.Num()) {
    

    ALostValleyGameMode* GameMode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();

    if(GameMode->roadmap->HasMail()) {
      if(dest == -1) {
        dest = GameMode->roadmap->GetMail();
      } else {
        dest = -1;
      }
      // Proceed to next path
      UE_LOG(LogTemp, Warning, TEXT("Delivery complete. Getting next delivery %i"), dest);
      instructions = GameMode->roadmap->Search(pawn->GetActorLocation(), dest);
      at = 0;
      NextPath();
    } else {
      GetWorld()->DestroyActor(this);
    }
  }
}

void APostmanController::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);


}

void APostmanController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) {
  NextPath();
}

void APostmanController::OnPossess(APawn* InPawn) {
  Super::OnPossess(InPawn);

  pawn = InPawn;

  ALostValleyGameMode* GameMode = (ALostValleyGameMode*)GetWorld()->GetAuthGameMode();
  instructions = GameMode->roadmap->Search(InPawn->GetActorLocation(), dest);
  at = 0;
  NextPath();
}
