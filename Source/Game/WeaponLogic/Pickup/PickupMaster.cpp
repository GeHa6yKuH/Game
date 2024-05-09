// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupMaster.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickupMaster::APickupMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PickupItemSK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupItem"));
	PickupItemSK->SetupAttachment(Root);

	PickupRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRadius"));
	PickupRadius->SetupAttachment(PickupItemSK);
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

void APickupMaster::Interact_Implementation(class ADefaultMainCharacter& CharacterPickingUp)
{
	UE_LOG(LogTemp, Warning, TEXT("pickup master iplemntation reached!"));
}
