// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameGameMode.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AKillEmAllGameMode : public AGameGameMode
{
	GENERATED_BODY()

public:

	virtual void PawnKilled(APawn* PawnKilled) override;
	
};
