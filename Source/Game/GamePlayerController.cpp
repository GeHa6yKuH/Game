// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"
#include "TimerManager.h"

void AGamePlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}