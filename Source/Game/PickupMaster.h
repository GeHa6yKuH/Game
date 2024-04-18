// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "PickupMaster.generated.h"

UCLASS()
class GAME_API APickupMaster : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UStaticMeshComponent* GetPickupItem() const { return PickupItem; }

	virtual void Interact_Implementation(class ADefaultMainCharacter& CharacterPickingUp);

private:
	UPROPERTY(VisibleAnyWhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PickupItem;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickupRadius;
};
