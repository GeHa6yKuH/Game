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

	void ReloadWeapon();

	void ResetCooldown();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanFire() const;

	UStaticMeshComponent* GetWeapon() const { return Weapon; }

	TEnumAsByte<WeaponType> GetWeaponType() const { return WeaponType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetMagazineBulletsAmount() const
    {
        return MagazineBulletsAmount;
    }

	UFUNCTION(BlueprintCallable, BlueprintPure)
    int GetTotalAmountOfBullets() const
    {
        return TotalAmountOfBullets;
    }

	UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetDelay() const
    {
        return Delay;
    }

	FVector SpreadTrace(FVector InitialVector) const;

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
	int TotalAmountOfBullets;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	int MaximumMagazineBulletsAmount;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	TEnumAsByte<WeaponType> WeaponType;

	UPROPERTY(EditDefaultsOnly, category = "Weapon Variables")
	float Delay;

	struct FTimerHandle TimerHandle;

	float SpreadFrom = -0.1;

	float SpreadTo = 0.1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, category = "Recoil Controller", BlueprintReadWrite)	
	float RecoilMulti = 1;

	UPROPERTY(EditAnyWhere, category = "Recoil Controller", BlueprintReadWrite)
	float SideRecoilMulti = 1;

	UPROPERTY(EditAnyWhere, category = "Recoil Controller", BlueprintReadWrite)
	float KickbackMulti = 1;

	UPROPERTY(EditAnyWhere, category = "Recoil Controller", BlueprintReadWrite)
	float KicksideMulti = 1;

	UPROPERTY(EditAnyWhere, category = "Recoil Controller", BlueprintReadWrite)
	float KickupMulti = 1;

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponIsFiring();

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStopsFiring();

};
