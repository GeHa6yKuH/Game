// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveDown.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UMoveDown : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveDown();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: 

	FVector OriginalLocation;

	UPROPERTY(EditAnywhere)
	bool ShouldDoorMove = false;

	UPROPERTY(EditAnywhere)
	bool ShouldDoorMoveDown = false;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	float MoveTime = 4.0f;

	UPROPERTY(EditAnywhere)
	bool DoorOpen = false;

	UPROPERTY(EditAnywhere)
	bool DoorClosed = true;

	UPROPERTY(EditAnywhere)
	FVector DoorOpenPosition;

	UPROPERTY(EditAnywhere)
	FVector DoorClosedPosition;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetShouldDoorMove(bool DoorShould);

	UFUNCTION()
	bool GetShouldDoorMove() const { return ShouldDoorMove; };

	UFUNCTION()
	void SetShouldDoorMoveDown(bool DoorShould);

	UFUNCTION()
	bool GetShouldDoorMoveDown() const { return ShouldDoorMoveDown; };

	UFUNCTION()
	bool IsDoorOpen() const { return DoorOpen; };

	UFUNCTION()
	bool IsDoorClosed() const { return DoorClosed; };

};
