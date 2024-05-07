// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AGameAIController::BeginPlay()
{
    Super::BeginPlay();
    // CurrentPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // SetFocus(CurrentPlayerPawn);
    if(AIBehavior)
    {
        RunBehaviorTree(AIBehavior);

        CurrentPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        BComp = GetBlackboardComponent();

        BComp->SetValueAsVector(TEXT("PlayerLocation"), CurrentPlayerPawn->GetActorLocation());
        BComp->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AGameAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
