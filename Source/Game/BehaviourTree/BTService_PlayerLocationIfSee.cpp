// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSee.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_PlayerLocationIfSee::UBTService_PlayerLocationIfSee()
{
    NodeName = TEXT("Player Location If See");
}

void UBTService_PlayerLocationIfSee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* CurrentPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if(CurrentPlayerPawn && AIController)
    {
        if(CurrentPlayerPawn && AIController->LineOfSightTo(CurrentPlayerPawn))
        {
           //OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), CurrentPlayerPawn->GetActorLocation());
        } else if(CurrentPlayerPawn)
        {
           // OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
}

