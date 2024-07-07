// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Grenade.h"
#include "GravitationGrenade.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AGravitationGrenade : public AGrenade
{
	GENERATED_BODY()

	public:

	bool ApplyForceToOverlappingActors() override;
	
};
