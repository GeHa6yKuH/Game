// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Components/StaticMeshComponent.h"
#include "DefaultMainCharacter.generated.h"


UCLASS()
class GAME_API ADefaultMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* RunAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* InteractAction;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning(const FInputActionValue& Value);
	
	void StopRunning(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

private:
	
	float Speed = 900;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARifleGun> RifleGunClass;

	UPROPERTY()
	class ARifleGun* RifleGun;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnWeapon(class AWeaponMaster* WeaponToSpawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isRunning;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mock;

	
};
