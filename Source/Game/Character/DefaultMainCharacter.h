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
	UInputAction* ShootTouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* TakeFirstWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* TakeSecondWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ReloadWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* TakeGrenadeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* SetGrenadeSpeedAction;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartRunning(const FInputActionValue& Value);
	
	void StopRunning(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void TakeFirstWeapon(const FInputActionValue& Value);

	void TakeSecondWeapon(const FInputActionValue& Value);

	void TakeGrenade(const FInputActionValue& Value);

	void ReloadWeapon(const FInputActionValue& Value);

	void SetGrenadeSpeed(const FInputActionValue& Value);

private:
	
	float RunSpeed = 900.f;

	float WalkSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly)
	float GrenadeThrowSpeed = 1500.f;

	UPROPERTY()
	class AWeaponMaster* CharacterWeapon;

	UPROPERTY()
	class AWeaponMaster* PreviousWeapon;

	UPROPERTY()
	class AGrenade* Grenade;

	UPROPERTY()
	class ADoorCard* DoorCard;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADoorCard> DoorCardClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARifleGun> RifleForAI;

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 10000000;

	UPROPERTY(VisibleAnyWhere)
	float HP;

	UPROPERTY(VisibleAnyWhere)
	FRotator NormalCharacterRotation;

	FName HeadBone = TEXT("spine_05");

	bool EquippingGrenade = false;

	bool IsMoving = false;

	bool IsLooking = false;

	FRotator MoveRotation;

	float MaxValueInSlideX = 30;

	float CountValueInSlideX = 0.f;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AGrenade> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AGrenade> GrenadeSecondClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AGrenade> EquippedGrenadeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool SecondGrenadeEquipped = false;

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

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsSliding = false;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHPPercent() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GrenadeEquipped = false;

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

	UFUNCTION(BlueprintCallable)
	void ShootTouch();

	void StopShootingTouch();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	class AWeaponMaster* GetCharacterWeapon() const { return CharacterWeapon; };

	UFUNCTION(BlueprintCallable)
	class AGrenade* GetGrenade() const { return Grenade; };

	class UInputMappingContext* const GetInputMapping() { return InputMapping; }

	UFUNCTION(BlueprintImplementableEvent)
	void AudioAndRecoilIfWeaponPresent();

	UFUNCTION(BlueprintImplementableEvent)
	void PushBoneBack(FName Bone, FVector BonePushVector);

	UFUNCTION()
	void SpawnCardInHandsBeforeAnim();

	UFUNCTION()
	void RemoveCardFromHandsAfterAnim();

	UPROPERTY(EditAnyWhere, category = "ADS", BlueprintReadWrite)
	bool isAiming;

	void HideWeapon();

	void DestroyGrenadeIfPresent();

	UFUNCTION()
	void Die();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetGrenadeThrowSpeed() const { return GrenadeThrowSpeed; }

	float GetHP() const { return HP; }

	void SetHP(float NewHP) { HP = NewHP; }

	void TakeGrenadeByClass(TSubclassOf<class AGrenade> GrenClass);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MouseX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MouseY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SidewayMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ForwardMovement;

	void AttachRifleToSocket(FName SocketName);

	void AddLookRotationInputs(float xVal, float yVal);

	void AddLookRotationInputsInSlide(float xVal, float yVal);
	
};
