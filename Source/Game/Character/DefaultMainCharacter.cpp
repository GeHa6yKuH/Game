// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMainCharacter.h"

#include "../WeaponLogic/Weapons/RifleGun.h"
#include "../WeaponLogic/Pickup/PickupMaster.h"
#include "../WeaponLogic/Weapons/WeaponMaster.h"
#include "../WeaponLogic/Grenade.h"
#include "../BehaviourTree/GameAIController.h"
#include "../GameModeAndController/GameGameMode.h"
#include "../GameModeAndController/KillEmAllGameMode.h"
#include "../Door/DoorCard.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/Vector2D.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
ADefaultMainCharacter::ADefaultMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mock = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mock"));
	Mock->SetupAttachment(GetMesh());
	
}

// Called when the game starts or when spawned
void ADefaultMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HP = MaxHP;

	NormalCharacterRotation = GetActorRotation();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (GetController()->IsA(AGameAIController::StaticClass()) && RifleForAI)
	{
		//SpawnWeapon(RifleForAI);
	}
	
	GetWorldTimerManager().SetTimer(IdleTimerHandle, this, &ADefaultMainCharacter::OnIdleTimeExceded, IdleTimeLimit, false);
}

void ADefaultMainCharacter::SpawnCardInHandsBeforeAnim()
{
	if (!DoorCard)
	{
		DoorCard = GetWorld()->SpawnActor<ADoorCard>(DoorCardClass);
		DoorCard->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("card_socket"));
		DoorCard->SetOwner(this);
	}
}

void ADefaultMainCharacter::RemoveCardFromHandsAfterAnim()
{
	if (DoorCard)
	{
		DoorCard->Destroy();
		DoorCard = nullptr;
	}
	if (CharacterWeapon)
	{
		FName SocketFiringPoseName = CharacterWeaponInt == 0 ? TEXT("NewWeaponSocket") : TEXT("Pistol_Socket");
		CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketFiringPoseName);
		SetIsPlayingAnimDoorFalse();
	}
}

void ADefaultMainCharacter::HideWeapon()
{
	if (CharacterWeapon)
	{
		FName SocketHolsterName = CharacterWeaponInt == 0 ? TEXT("rifle_holster") : TEXT("pistol_holster");
		CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketHolsterName);
	}
}

// Called every frame
void ADefaultMainCharacter::Tick(float DeltaTime)
{
	if (isRunning)
	{
		CanSlide = true;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	} else if (IsSliding)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		CanSlide = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (!IsSliding)
	{
		CountValueInSlideX = 0.f;
	}
	
	SetFlashlightCorrectPosition();

    if (!IsMoving && SidewayMovement != 0.0f)
    {
        SidewayMovement = 0.0f;
    }

    if (!IsLooking && (MouseX != 0.0f || MouseY != 0.0f))
    {
        MouseX = 0.0f;
        MouseY = 0.0f;
    }

    IsMoving = false;
    IsLooking = false;
}

// Called to bind functionality to input
void ADefaultMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Move);

		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Look);

		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Started, this, &ADefaultMainCharacter::StartRunning);
		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::StopRunning);

		EnhancedInputComp->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Shoot);
		EnhancedInputComp->BindAction(ShootAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::StopShooting);

		EnhancedInputComp->BindAction(ShootTouchAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ShootTouch);
		EnhancedInputComp->BindAction(ShootTouchAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::StopShootingTouch);

		EnhancedInputComp->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Interact);

		EnhancedInputComp->BindAction(TakeFirstWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::TakeFirstWeapon);
		EnhancedInputComp->BindAction(TakeSecondWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::TakeSecondWeapon);

		EnhancedInputComp->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ReloadWeapon);

		EnhancedInputComp->BindAction(TakeGrenadeAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::TakeGrenade);

		EnhancedInputComp->BindAction(SetGrenadeSpeedAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::SetGrenadeSpeed);


		// --------------------------------------------------------------------------------------------------------------------------------


		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Started, this, &ADefaultMainCharacter::ResetIdleTimer);
		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);
		EnhancedInputComp->BindAction(ShootAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(ShootTouchAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);
		EnhancedInputComp->BindAction(ShootTouchAction, ETriggerEvent::Completed, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(TakeFirstWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);
		EnhancedInputComp->BindAction(TakeSecondWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(TakeGrenadeAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);

		EnhancedInputComp->BindAction(SetGrenadeSpeedAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ResetIdleTimer);
	}
}

void ADefaultMainCharacter::SetGrenadeSpeed(const FInputActionValue& Value)
{
	if (GrenadeEquipped && isAiming)
	{
		GrenadeThrowSpeed += Value.Get<float>() * 50;
	}
}

void ADefaultMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVal = Value.Get<FVector2D>();

	ForwardMovement = !IsSliding ? MoveVal.Y : 1;
	SidewayMovement = !IsSliding ? MoveVal.X : 0;
	
	if (!IsSliding)
	{
		MoveRotation = GetController()->GetControlRotation();
	}

	const FRotator YawRotation(0.f, MoveRotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Forward, ForwardMovement);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Right, SidewayMovement);

	IsMoving = true;
}

void ADefaultMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		if (!IsSliding)
		{
			AddLookRotationInputs(LookAxisValue.X, LookAxisValue.Y);
		} else
		{
			AddLookRotationInputsInSlide(LookAxisValue.X, LookAxisValue.Y);
		}
	}
	
	IsLooking = true;
}

void ADefaultMainCharacter::Shoot()
{
	if(CharacterWeapon && (!isRunning || IsSliding))
	{
		CharacterWeapon->PullTrigger(false);
		if (CharacterWeapon->GetMagazineBulletsAmount() > 0 && !CharacterWeapon->IsReloading())
		{
			CharacterWeapon->WeaponIsFiring();
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No weapon in hands to shoot!"))
	}
}

void ADefaultMainCharacter::StopShooting()
{
	if (CharacterWeapon)
	{
		CharacterWeapon->WeaponStopsFiring();
	}
}

void ADefaultMainCharacter::ShootTouch()
{
	if(CharacterWeapon && (!isRunning || IsSliding))
	{
		CharacterWeapon->PullTrigger(true);
		if (CharacterWeapon->GetMagazineBulletsAmount() > 0 && !CharacterWeapon->IsReloading())
		{
			CharacterWeapon->WeaponIsFiring();
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No weapon in hands to shoot!"))
	}
}

void ADefaultMainCharacter::StopShootingTouch()
{
	if (CharacterWeapon)
	{
		CharacterWeapon->WeaponStopsFiring();
	}
}

void ADefaultMainCharacter::StartRunning(const FInputActionValue& Value)
{
	if (!isAiming)
	{
		isRunning = true;
	}
}

void ADefaultMainCharacter::StopRunning(const FInputActionValue& Value)
{
	isRunning = false;
}

void ADefaultMainCharacter::Interact(const FInputActionValue& Value)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APickupMaster::StaticClass());
	if (OverlappingActors.Num() > 0)
	{
		APickupMaster* OverlappingPickupMaster = Cast<APickupMaster>(OverlappingActors[0]);
		if (OverlappingPickupMaster)
		{
			DestroyGrenadeIfPresent();
			OverlappingPickupMaster->Interact_Implementation(*this);
		} else { UE_LOG(LogTemp, Error, TEXT("Unable to cast to PickupMaster!")); }
	} else { UE_LOG(LogTemp, Error, TEXT("No Overlapping Actors found!")); }
}

void ADefaultMainCharacter::TakeFirstWeapon(const FInputActionValue& Value)
{
	if (!IsPlayingAnimDoor)
	{	
		if (CharacterWeapon && CharacterWeapon->GetWeaponType() == 0)
		{
			if (!GrenadeEquipped)
			{
				return;
			}
			CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("NewWeaponSocket"));
			DestroyGrenadeIfPresent();
		} else if (CharacterWeapon && PreviousWeapon)
		{
			AWeaponMaster* SwitchWeapon = PreviousWeapon;
			PreviousWeapon = CharacterWeapon;
			PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("pistol_holster"));
			CharacterWeapon = SwitchWeapon;
			CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("NewWeaponSocket"));
			USkeletalMeshComponent* WeaponMesh = CharacterWeapon->GetWeapon();
			if (WeaponMesh)
			{
				Mock = WeaponMesh;
			}
			CharacterWeaponInt = 0;
			DestroyGrenadeIfPresent();
		}
	}
}

void ADefaultMainCharacter::TakeSecondWeapon(const FInputActionValue& Value)
{
	if (!IsPlayingAnimDoor)
	{ 
		if (CharacterWeapon && CharacterWeapon->GetWeaponType() == 1)
		{
			if (!GrenadeEquipped)
			{
				return;
			}
			CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pistol_Socket"));
			DestroyGrenadeIfPresent();
		} else if (CharacterWeapon && PreviousWeapon)
		{
			AWeaponMaster* SwitchWeapon = PreviousWeapon;
			PreviousWeapon = CharacterWeapon;
			PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("rifle_holster"));
			CharacterWeapon = SwitchWeapon;
			CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pistol_Socket"));
			USkeletalMeshComponent* WeaponMesh = CharacterWeapon->GetWeapon();
			if (WeaponMesh)
			{
				Mock = WeaponMesh;
			}
			CharacterWeaponInt = 1;
			DestroyGrenadeIfPresent();
		}
	}
}

void ADefaultMainCharacter::TakeGrenade(const FInputActionValue& Value)
{
	if (!IsPlayingAnimDoor && GrenadeClass && !isRunning && !EquippingGrenade)
	{
		EquippingGrenade = true;
		if (!Grenade || SecondGrenadeEquipped)
		{
			DestroyGrenadeIfPresent();
			TakeGrenadeByClass(GrenadeClass);
			SecondGrenadeEquipped = false;
			EquippedGrenadeClass = GrenadeClass;
		} else if (!SecondGrenadeEquipped)
		{
			DestroyGrenadeIfPresent();
			TakeGrenadeByClass(GrenadeSecondClass);
			SecondGrenadeEquipped = true;
			EquippedGrenadeClass = GrenadeSecondClass;
		}
		EquippingGrenade = false;
	}
}

void ADefaultMainCharacter::DestroyGrenadeIfPresent()
{
	if (Grenade)
	{
		Grenade->Destroy();
		Grenade = nullptr;
		GrenadeEquipped = false;
	}
}

void ADefaultMainCharacter::TakeGrenadeByClass(TSubclassOf<class AGrenade> GrenClass)
{
	HideWeapon();
	Grenade = GetWorld()->SpawnActor<AGrenade>(GrenClass);
	Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Grenade"));
	Grenade->SetOwner(this);
	GrenadeEquipped = true;
}

void ADefaultMainCharacter::AttachRifleToSocket(FName SocketName)
{
	if (CharacterWeapon && CharacterWeapon->GetWeaponType() == 0)
	{
		CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	}
}

void ADefaultMainCharacter::AddLookRotationInputs(float xVal, float yVal)
{
	AddControllerYawInput(xVal);
	AddControllerPitchInput(yVal);
	MouseX = xVal;
	MouseY = yVal;
}

void ADefaultMainCharacter::AddLookRotationInputsInSlide(float xVal, float yVal)
{
	CountValueInSlideX += xVal;

	AddControllerPitchInput(yVal);
	MouseY = yVal;

	if (CountValueInSlideX < MaxValueInSlideX && CountValueInSlideX > -MaxValueInSlideX)
	{
		AddControllerYawInput(xVal);
		MouseX = xVal;
	}
}

void ADefaultMainCharacter::ResetIdleTimer()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Stop(0.3, IdleAFKAnimation);
    }

	GetWorldTimerManager().ClearTimer(IdleTimerHandle);
    GetWorldTimerManager().SetTimer(IdleTimerHandle, this, &ADefaultMainCharacter::OnIdleTimeExceded, IdleTimeLimit, false);
}

void ADefaultMainCharacter::OnIdleTimeExceded()
{
	PlayAFKAnimation();
}

void ADefaultMainCharacter::ReloadWeapon(const FInputActionValue& Value)
{
	if (CharacterWeapon)
	{
		CharacterWeapon->ReloadWeapon();
	}
}

void ADefaultMainCharacter::SpawnWeapon(TSubclassOf<AWeaponMaster> WeaponClass)
{
	if(WeaponClass)
	{
		HasWeapon = true;
		if(CharacterWeapon)
		{
			PreviousWeapon = CharacterWeapon;
			FName SocketName = CharacterWeaponInt == 0 ? TEXT("rifle_holster") : TEXT("pistol_holster");
			PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		}
		CharacterWeapon = GetWorld()->SpawnActor<AWeaponMaster>(WeaponClass);
		if (CharacterWeapon && GetMesh())
		{
			CharacterWeaponInt = CharacterWeapon->GetWeaponType();
			if (CharacterWeaponInt >= 0)
			{
				FName SocketName = CharacterWeaponInt == 0 ? TEXT("NewWeaponSocket") : TEXT("Pistol_Socket");
				CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
				CharacterWeapon->SetOwner(this);
				USkeletalMeshComponent* WeaponMesh = CharacterWeapon->GetWeapon();
				if (WeaponMesh)
				{
					Mock = WeaponMesh;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn WeaponPickup"));
		}
	}
}

bool ADefaultMainCharacter::IsDead() const
{
	return HP <= 0;
}

float ADefaultMainCharacter::GetHPPercent() const
{
	return HP / MaxHP;
}

float ADefaultMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(DamageApplied > 0 && HP > 0)
	{
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
        {
            const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

			if(PointDamageEvent)
			{
				FName BoneHit = PointDamageEvent->HitInfo.BoneName;

				FVector HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
        		FVector TraceStart = PointDamageEvent->HitInfo.TraceStart;
        		FVector HitDirection = (HitLocation - TraceStart).GetSafeNormal();

				PushBoneBack(BoneHit, HitDirection);
				if (BoneHit == HeadBone)
            	{
					UE_LOG(LogTemp, Warning, TEXT("Headshot!"));
            	    DamageApplied *= 5.0f;
            	}
			}
        }
		DamageApplied = FMath::Min(HP, DamageApplied);
		HP -= DamageApplied;

		if(IsDead())
		{
			Die();
		}
	}

	return DamageApplied;
}

void ADefaultMainCharacter::Die()
{
    AGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AGameGameMode>();
    if (GameMode)
    {
        GameMode->PawnKilled(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("error no gamemode found!"));
    }

    UE_LOG(LogTemp, Warning, TEXT("%s is killed"), *GetName());
    
    DetachFromControllerPendingDestroy();
    
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->StopMovementImmediately();
        MovementComponent->DisableMovement();
    }

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    USkeletalMeshComponent* CharMesh = GetMesh();
    if (CharMesh)
    {
        CharMesh->SetSimulatePhysics(true);
        CharMesh->WakeAllRigidBodies();
    }

    if (Controller)
    {
        Controller->UnPossess();
        Controller->StopMovement();
    }
}
