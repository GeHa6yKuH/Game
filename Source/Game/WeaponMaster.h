// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

UENUM(BlueprintType)
	enum WeaponType
	{
       Rifle UMETA(DisplayName = "Rifle"),
       Pistol UMETA(DisplayName = "Pistol"),
	};

UCLASS()
class GAME_API AWeaponMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMaster();

	void PullTrigger();

	UStaticMeshComponent* GetWeapon() const { return Weapon; }

private:

	UPROPERTY(VisibleAnyWhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	float Damage;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	float MaxRange;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	int MagazineBulletsAmount;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	TEnumAsByte<WeaponType> WeaponType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
