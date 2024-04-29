// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponMaster.h"
#include "Engine/DamageEvents.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponMaster::AWeaponMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Root);

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
	FVector End = Location + Rotation.Vector() * MaxRange;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

	if(bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		AActor* ActorHit = HitResult.GetActor();
		if(ActorHit)
		{
			ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("no successful line tracing!"));
	}
}

