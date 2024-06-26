// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class GAME_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnyWhere)
	USceneComponent* RootScene;

	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* Grenade;

	UPROPERTY(EditAnyWhere)
	float ExplosionRadius = 700.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	TArray<AActor*> ActorsToIgnore;

	FVector ExplosionLocation;

	TArray<AActor*> ActorsInExplosionRadius;

	bool IsExploding = false;

	float Damage = 50.f;

	UPROPERTY(EditAnyWhere)
	float ForceMagnitude = 20000.f;

	UPROPERTY(EditAnyWhere)
	float MagnitudeIrrelatableDistance = 150.f;

	TMap<class ACharacter*, FRotator> OriginalRotations;

	TMap<ACharacter*, FVector> OriginalPositions;

	int32 ActorsInExplosionRadiusSize;

	int32 ActorsInExplosionRadiusCount = 0;

	FVector CharacterMeshLocalLocation{-30.f, -6.386468, -87.289543};

    FRotator CharacterMeshLocalRotation{0.f, -90.f, 0.f};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	UFUNCTION(BlueprintCallable)
	bool Explode();

	UFUNCTION(BlueprintCallable)
	bool GetOverlappingActorsInRadiusOfExplosion();

	UFUNCTION(BlueprintCallable)
	bool ApplyForceToOverlappingActors();

	UFUNCTION(BlueprintCallable)
	void ReturnActorsToNormalState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsExploding() const { return IsExploding; };

};
