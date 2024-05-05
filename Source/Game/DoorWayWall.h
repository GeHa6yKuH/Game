// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorWayWall.generated.h"

UCLASS()
class GAME_API ADoorWayWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorWayWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UActorComponent* ActorComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PannelMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SecondPannel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UDoorWallBoxComponent* DoorWallBoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UDoorWallBoxComponent* BoxCompForSecodPannel;

};
