// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMainCharacter.h"

#include "../WeaponLogic/Weapons/RifleGun.h"
#include "../WeaponLogic/Pickup/PickupMaster.h"
#include "../WeaponLogic/Weapons/WeaponMaster.h"
#include "../BehaviourTree/GameAIController.h"
#include "../GameModeAndController/GameGameMode.h"
#include "../GameModeAndController/KillEmAllGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/Vector2D.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"


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

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (GetController()->IsA(AGameAIController::StaticClass()) && RifleForAI)
	{
		SpawnWeapon(RifleForAI);
	}
	
}

// Called every frame
void ADefaultMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRunning)
	{
		CanSlide = true;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else
	{
		CanSlide = false;
		GetCharacterMovement()->MaxWalkSpeed = 450;
	}
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

	EnhancedInputComp->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Interact);

	EnhancedInputComp->BindAction(TakeFirstWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::TakeFirstWeapon);
	EnhancedInputComp->BindAction(TakeSecondWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::TakeSecondWeapon);

	EnhancedInputComp->BindAction(ReloadWeaponAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::ReloadWeapon);
}

}

void ADefaultMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVal = Value.Get<FVector2D>();
	
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Forward, MoveVal.Y);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Right, MoveVal.X);
}

void ADefaultMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
	
}

void ADefaultMainCharacter::Shoot()
{
	if(CharacterWeapon && !isRunning)
	{
		CharacterWeapon->PullTrigger();
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

void ADefaultMainCharacter::StartRunning(const FInputActionValue& Value)
{
	isRunning = true;
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
			OverlappingPickupMaster->Interact_Implementation(*this);
		} else { UE_LOG(LogTemp, Error, TEXT("Unable to cast to PickupMaster!")); }
	} else { UE_LOG(LogTemp, Error, TEXT("No Overlapping Actors found!")); }
}

void ADefaultMainCharacter::TakeFirstWeapon(const FInputActionValue& Value)
{
	if (CharacterWeapon && CharacterWeapon->GetWeaponType() == 0)
	{
		return;
	} else if (CharacterWeapon && PreviousWeapon)
	{
		AWeaponMaster* SwitchWeapon = PreviousWeapon;
		PreviousWeapon = CharacterWeapon;
		PreviousWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("pistol_holster"));
		CharacterWeapon = SwitchWeapon;
		CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
		USkeletalMeshComponent* WeaponMesh = CharacterWeapon->GetWeapon();
		if (WeaponMesh)
		{
			Mock = WeaponMesh;
		}
		CharacterWeaponInt = 0;
	}
}

void ADefaultMainCharacter::TakeSecondWeapon(const FInputActionValue& Value)
{
	if (CharacterWeapon && CharacterWeapon->GetWeaponType() == 1)
	{
		return;
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
	}
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
				FName SocketName = CharacterWeaponInt == 0 ? TEXT("Weapon") : TEXT("Pistol_Socket");
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
		DamageApplied = FMath::Min(HP, DamageApplied);
		HP -= DamageApplied;
		UE_LOG(LogTemp, Warning, TEXT("current health after shot for %s: %f"), *GetName(), HP);

		if(IsDead())
		{
			AGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AGameGameMode>();
			if (GameMode)
			{
				GameMode->PawnKilled(this);
			} else
			{
				UE_LOG(LogTemp, Error, TEXT("error no gamemode found!"));
			}
			

			UE_LOG(LogTemp, Warning, TEXT("%s is killed"), *GetName());
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetSimulatePhysics(true);
		}
	}

	return DamageApplied;
}
