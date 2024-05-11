// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponMaster.h"
#include "Engine/DamageEvents.h"
#include "../../Character/DefaultMainCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponMaster::AWeaponMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponSK->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponMaster::PullTrigger()
{
	if (CanFire() && !IsReloading() && MagazineBulletsAmount > 0)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Owner is null!"));
			return;
		}
		AController* OwnerController = OwnerPawn->GetController();
		if (OwnerController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Owner controller is null!"));
			return;
		}

		FVector Location;
		FRotator Rotation;
		OwnerController->GetPlayerViewPoint(Location, Rotation);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		FVector End = Location + (SpreadTrace(Rotation.Vector()) * MaxRange);

		ADefaultMainCharacter* CurrentShootingMainCharacter = Cast<ADefaultMainCharacter>(GetOwner());

		if (CurrentShootingMainCharacter)
		{
			CurrentShootingMainCharacter->AudioAndRecoilIfWeaponPresent();
			MagazineBulletsAmount--;
		}
		

		bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponMaster::ResetCooldown, Delay, false);

		if(bSuccess)
		{
			FVector ShotDirection = -Rotation.Vector();
			DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AActor* ActorHit = HitResult.GetActor();
			if(ActorHit)
			{
				ADefaultMainCharacter* CharacterHit = Cast<ADefaultMainCharacter>(ActorHit);
				if(CharacterHit && CharacterHit->IsDead())
				{
					return;
				}
				ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
	}	else if (CanFire() && TotalAmountOfBullets > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please reload!"));
	}
	
}

void AWeaponMaster::ReloadWeapon()
{
	BulletsToReload = MaximumMagazineBulletsAmount - MagazineBulletsAmount;
	CanBeReloaded = BulletsToReload > 0 && TotalAmountOfBullets > 0;
	if (CanBeReloaded && BulletsToReload > 0 && TotalAmountOfBullets >= BulletsToReload)
	{
		TotalAmountOfBullets -= BulletsToReload;
		MagazineBulletsAmount = MaximumMagazineBulletsAmount;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponMaster::ResetReloadCooldown, ReloadDelay, false);
	} else if (CanBeReloaded && BulletsToReload > 0 && TotalAmountOfBullets > 0)
	{
		MagazineBulletsAmount += TotalAmountOfBullets;
		TotalAmountOfBullets = 0;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponMaster::ResetReloadCooldown, ReloadDelay, false);
	}
}

void AWeaponMaster::ResetCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AWeaponMaster::ResetReloadCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

bool AWeaponMaster::CanFire() const
{
	if (TimerHandle.IsValid())
	{
		return !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle);
	}
	return true;
}

bool AWeaponMaster::IsReloading() const
{
	if (ReloadTimerHandle.IsValid())
	{
		return GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle);
	}
	return false;
}

FVector AWeaponMaster::SpreadTrace(FVector InitialVector)
{
	ADefaultMainCharacter* CurrentShootingMainCharacter = Cast<ADefaultMainCharacter>(GetOwner());

	if (CurrentShootingMainCharacter)
	{
		AimingMultiplier = CurrentShootingMainCharacter->isAiming ? 0.1 : 1;
		float SpreadFromWithMulti = SpreadFrom * AimingMultiplier;
		float SpreadToWithMulti = SpreadTo * AimingMultiplier;
		InitialVector.X += FMath::RandRange(SpreadFromWithMulti, SpreadToWithMulti);
		InitialVector.Y += FMath::RandRange(SpreadFromWithMulti, SpreadToWithMulti);
		InitialVector.Z += FMath::RandRange(SpreadFromWithMulti, SpreadToWithMulti);
	}

	return InitialVector;
}

