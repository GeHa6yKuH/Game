// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorCard.h"

// Sets default values
ADoorCard::ADoorCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Card"));
	CardMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ADoorCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

