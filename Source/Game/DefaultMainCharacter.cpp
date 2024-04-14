// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMainCharacter.h"
#include "Gun.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
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

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
	Gun->SetOwner(this);

	UStaticMeshComponent* GunMesh = Gun->GetWeapon();
	if (GunMesh)
	{
		Mock = GunMesh;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
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

void ADefaultMainCharacter::Shoot(const FInputActionValue& Value)
{
	Gun->PullTrigger();
}

void ADefaultMainCharacter::StartRunning(const FInputActionValue& Value)
{
	isRunning = true;
}

void ADefaultMainCharacter::StopRunning(const FInputActionValue& Value)
{
	isRunning = false;
}
