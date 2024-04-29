// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AGameAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

private:

	APawn* CurrentPlayerPawn;

	UPROPERTY(EditAnyWhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY()
	class UBlackboardComponent* BComp;

protected:

	virtual void BeginPlay() override;
};
