// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"
#include "WeaponMaster.h"
#include "DefaultMainCharacter.h"
#include "DrawDebugHelpers.h"

void AWeaponPickup::Interact_Implementation(class ADefaultMainCharacter& CharacterPickingUp)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon Pickup interact reached!"));
    CharacterPickingUp.SpawnWeapon(WeaponToSpawn);
    GetWorld()->DestroyActor(this);
}
