// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PickupMaster.h"

// Sets default values
APickupMaster::APickupMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PickupItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItem"));
	PickupItem->SetupAttachment(Root);

	PickupRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRadius"));
	PickupRadius->SetupAttachment(PickupItem);
}

// Called when the game starts or when spawned
void APickupMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

