// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Grenade.h"
#include "GravitationalPushGrenade.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AGravitationalPushGrenade : public AGrenade
{
	GENERATED_BODY()

	bool ApplyForceToOverlappingActors() override;

	public:

	AGravitationalPushGrenade();
};
