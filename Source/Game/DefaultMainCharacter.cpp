// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMainCharacter.h"

#include "RifleGun.h"
#include "PickupMaster.h"
#include "WeaponMaster.h"
#include "GameAIController.h"
#include "GameGameMode.h"
#include "KillEmAllGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/Vector2D.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values
ADefaultMainCharacter::ADefaultMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mock"));
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
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else
	{
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

	EnhancedInputComp->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADefaultMainCharacter::Interact);
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
	if(CharacterWeapon)
	{
		CharacterWeapon->PullTrigger();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No weapon in hands to shoot!"))
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

void ADefaultMainCharacter::SpawnWeapon(TSubclassOf<AWeaponMaster> WeaponClass)
{
	if(WeaponClass)
	{
		if(CharacterWeapon)
		{
			GetWorld()->DestroyActor(CharacterWeapon);
		}
		CharacterWeapon = GetWorld()->SpawnActor<AWeaponMaster>(WeaponClass);
		if (CharacterWeapon && GetMesh())
		{
			CharacterWeaponInt = CharacterWeapon->GetWeaponType();
			FName SocketName = CharacterWeaponInt == 0 ? TEXT("Weapon") : TEXT("Pistol_Socket");
			CharacterWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			CharacterWeapon->SetOwner(this);
			UStaticMeshComponent* WeaponMesh = CharacterWeapon->GetWeapon();
			if (WeaponMesh)
			{
				Mock = WeaponMesh;
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
			}

			UE_LOG(LogTemp, Warning, TEXT("%s is killed"), *GetName());
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetSimulatePhysics(true);
		}
	}

	return DamageApplied;
}
