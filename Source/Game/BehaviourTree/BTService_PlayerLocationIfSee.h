// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerLocationIfSee.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTService_PlayerLocationIfSee : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTService_PlayerLocationIfSee();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
