// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveDown.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UMoveDown::UMoveDown()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveDown::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
	DoorOpenPosition = OriginalLocation + FVector(0.f, 0.f, 400.f);
	DoorClosedPosition = OriginalLocation;

}


// Called every frame
void UMoveDown::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DoorOpen = GetOwner()->GetActorLocation().Z >= DoorOpenPosition.Z;
	DoorClosed = GetOwner()->GetActorLocation().Z <= DoorClosedPosition.Z;

	if (DoorOpen)
	{
		SetShouldDoorMove(false);
		FTimerHandle TimerMemberHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerMemberHandle, this, &UMoveDown::CloseDoor, 5.0f, false);
	}

	if (DoorClosed)
	{
		SetShouldDoorMoveDown(false);
	}
	
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = CurrentLocation;

	if (ShouldDoorMove && !DoorOpen)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Door is going upwards!"));
		TargetLocation = CurrentLocation + MoveOffset;
		float Speed = MoveOffset.Length() / MoveTime;
		GetOwner()->SetActorLocation(FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed));
	}

	if (ShouldDoorMoveDown && !DoorClosed)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Door is closing!"));
		TargetLocation = CurrentLocation - MoveOffset;
		float Speed = MoveOffset.Length() / MoveTime;
		GetOwner()->SetActorLocation(FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed));
	}
}

void UMoveDown::SetShouldDoorMove(bool DoorShould)
{
	ShouldDoorMove = DoorShould;
}

void UMoveDown::SetShouldDoorMoveDown(bool DoorShould)
{
	ShouldDoorMoveDown = DoorShould;
}
