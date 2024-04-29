// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "DefaultMainCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);


    if(OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())
    {
        ADefaultMainCharacter* PlayerPawn = Cast<ADefaultMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
        if(PlayerPawn)
        {
            PlayerPawn->Shoot();
        } else
        {
            return EBTNodeResult::Failed;
        }
    } else
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}
