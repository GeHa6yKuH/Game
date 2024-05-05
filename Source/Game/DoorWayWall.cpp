// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWayWall.h"
#include "DoorWallBoxComponent.h"
#include "DoorWallBoxComponent.h"

// Sets default values
ADoorWayWall::ADoorWayWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWall"));
	StaticMesh->SetupAttachment(RootComponent);

	PannelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PannelMesh"));
	PannelMesh->SetupAttachment(RootComponent);

	SecondPannel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPannel"));
	SecondPannel->SetupAttachment(RootComponent);

	DoorWallBoxComp = CreateDefaultSubobject<UDoorWallBoxComponent>(TEXT("DoorWallBoxComponent"));
	DoorWallBoxComp->SetupAttachment(PannelMesh);

	BoxCompForSecodPannel = CreateDefaultSubobject<UDoorWallBoxComponent>(TEXT("BoxCompForSecodPannel"));
	BoxCompForSecodPannel->SetupAttachment(SecondPannel);

}

// Called when the game starts or when spawned
void ADoorWayWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorWayWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
