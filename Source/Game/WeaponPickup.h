// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupMaster.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AWeaponPickup : public APickupMaster
{
	GENERATED_BODY()

public:

	virtual void Interact_Implementation(class ADefaultMainCharacter& CharacterPickingUp);

private:

	UPROPERTY(VisibleAnywhere)
	class AWeaponMaster* WeaponToSpawn;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;
	
};
