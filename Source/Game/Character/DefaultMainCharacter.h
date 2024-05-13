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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* TakeFirstWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* TakeSecondWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ReloadWeaponAction;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning(const FInputActionValue& Value);
	
	void StopRunning(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void TakeFirstWeapon(const FInputActionValue& Value);

	void TakeSecondWeapon(const FInputActionValue& Value);

	void ReloadWeapon(const FInputActionValue& Value);

private:
	
	float Speed = 900;

	UPROPERTY()
	class AWeaponMaster* CharacterWeapon;

	UPROPERTY()
	class AWeaponMaster* PreviousWeapon;

	UPROPERTY()
	class ADoorCard* DoorCard;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADoorCard> DoorCardClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARifleGun> RifleForAI;

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 100;

	UPROPERTY(VisibleAnyWhere)
	float HP;

public:

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	int CharacterWeaponInt = -1;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	bool HasWeapon = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	bool IsPlayingAnimDoor = false;

	void SetIsPlayingAnimDoorTrue() { IsPlayingAnimDoor = true; }

	void SetIsPlayingAnimDoorFalse() { IsPlayingAnimDoor = false; };

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class UAnimMontage* PistolEquipAnimation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool CanSlide = false;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHPPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnWeapon(TSubclassOf<class AWeaponMaster> WeaponClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isRunning;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Mock;

	UFUNCTION(BlueprintCallable)
	void Shoot();

	void StopShooting();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	class AWeaponMaster* GetCharacterWeapon() const { return CharacterWeapon; };

	class UInputMappingContext* const GetInputMapping() { return InputMapping; }

	UFUNCTION(BlueprintImplementableEvent)
	void AudioAndRecoilIfWeaponPresent();

	UFUNCTION()
	void SpawnCardInHandsBeforeAnim();

	UFUNCTION()
	void RemoveCardFromHandsAfterAnim();

	UPROPERTY(EditAnyWhere, category = "ADS", BlueprintReadWrite)
	bool isAiming;

	void HideWeapon();


	
};
