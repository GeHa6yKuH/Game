// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorTriggeringPannel.generated.h"

UCLASS()
class GAME_API ADoorTriggeringPannel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorTriggeringPannel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PannelMesh;

	UPROPERTY(VisibleAnywhere)
	class UDoorWallBoxComponent* DoorWallBoxComp;

	class UMoveDown* MoveDownComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMoveDownComp(class UMoveDown* NewMoveDownComp);

};
