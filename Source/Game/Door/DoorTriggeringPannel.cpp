// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTriggeringPannel.h"
#include "MoveDown.h"
#include "DoorWallBoxComponent.h"

// Sets default values
ADoorTriggeringPannel::ADoorTriggeringPannel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PannelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PannelMesh"));
	PannelMesh->SetupAttachment(RootComponent);

	DoorWallBoxComp = CreateDefaultSubobject<UDoorWallBoxComponent>(TEXT("DoorWallBoxComponent"));
	DoorWallBoxComp->SetupAttachment(PannelMesh);

}

// Called when the game starts or when spawned
void ADoorTriggeringPannel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorTriggeringPannel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorTriggeringPannel::SetMoveDownComp(class UMoveDown* NewMoveDownComp)
{
    MoveDownComp = NewMoveDownComp;
}

