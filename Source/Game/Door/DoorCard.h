// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorCard.generated.h"

UCLASS()
class GAME_API ADoorCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnyWhere)
	USceneComponent* Root;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
