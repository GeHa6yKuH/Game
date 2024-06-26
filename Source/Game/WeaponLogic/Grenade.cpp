// Fill out your copyright notice in the Description page of Project Settings.


#include "./Grenade.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Character/DefaultMainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootScene);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootScene);

	Grenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade"));
	Grenade->SetupAttachment(Sphere);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsExploding)
	{
		ApplyForceToOverlappingActors();
	}
}

bool AGrenade::Explode()
{
	if (this)
	{
		GetOverlappingActorsInRadiusOfExplosion();
		IsExploding = true;
		// Destroy();
		if (Grenade)
		{
			Grenade->SetVisibility(false);
			Grenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		FTimerHandle AfterExplosionTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(AfterExplosionTimerHandle, this, &AGrenade::ReturnActorsToNormalState, 5.f, false);
	}
	return true;
}

bool AGrenade::GetOverlappingActorsInRadiusOfExplosion()
{
	ExplosionLocation = Sphere->GetComponentLocation();

	DrawDebugSphere(
		GetWorld(),
		ExplosionLocation,
		ExplosionRadius,
		30,
		FColor::Emerald,
		true,
		3.f
	);

	bool OverlappingFound = UKismetSystemLibrary::SphereOverlapActors(
		this,
		ExplosionLocation,
		ExplosionRadius,
		ObjectTypes,
		APawn::StaticClass(),
		ActorsToIgnore,
		ActorsInExplosionRadius
	);

	return true;
}

bool AGrenade::ApplyForceToOverlappingActors()
{
    if (!ActorsInExplosionRadius.IsEmpty())
    {
        ActorsInExplosionRadiusSize = ActorsInExplosionRadius.Num();
        for (AActor* Actor : ActorsInExplosionRadius)
        {
            APawn* Pawn = Cast<APawn>(Actor);
            if (Pawn)
            {
                FVector Direction = ExplosionLocation - Pawn->GetActorLocation();
                float Distance = Direction.Size();

                if (Distance < MagnitudeIrrelatableDistance)
                {
                    continue;
                }

                Direction.Normalize();

                double ValueToClamp = Distance / 1000;

                float DistanceFactor = FMath::Clamp(ValueToClamp, 0.0f, 1.0f);
                
                FVector Force = Direction * ForceMagnitude * DistanceFactor;

                ACharacter* Character = Cast<ACharacter>(Pawn);
                if (Character && Character->GetMesh())
                {
                    if (ActorsInExplosionRadiusCount < ActorsInExplosionRadiusSize)
                    {
                        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
                        if (AnimInstance)
                        {
                            AnimInstance->StopAllMontages(0.0f);
                        }

                        FRotator Rotation = Character->GetActorRotation();
                        FVector Location = Character->GetActorLocation();

                        OriginalRotations.Add(Character, Rotation);
                        OriginalPositions.Add(Character, Location);

                        if (Character->GetController())
                        {
                            Character->GetController()->StopMovement();
                        }

                        Character->GetMesh()->SetEnableGravity(false);
                        if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
                        {
                            MoveComp->GravityScale = 0.f;
                        }

                        Character->GetMesh()->SetSimulatePhysics(true);
                        ActorsInExplosionRadiusCount++;
                    }

                    Character->GetMesh()->AddForce(Force);
                }
            }
        }
    }
    return true;
}

void AGrenade::ReturnActorsToNormalState()
{
    Destroy();
    if (!ActorsInExplosionRadius.IsEmpty())
    {
        for (AActor* Actor : ActorsInExplosionRadius)
        {
            APawn* Pawn = Cast<APawn>(Actor);
            if (Pawn)
            {
                ADefaultMainCharacter* Character = Cast<ADefaultMainCharacter>(Pawn);
                USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
                if (Character && CharacterMesh)
                {
                    if (OriginalPositions.Contains(Character) && OriginalRotations.Contains(Character))
                    {
                        // Clear any pending physics velocities
                        CharacterMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
                        CharacterMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

                        // Disable physics simulation
                        CharacterMesh->SetSimulatePhysics(false);

                        // Set the character's location to the current world location of the mesh
                        FVector CurrentWorldLocation = CharacterMesh->GetComponentLocation();

                        DrawDebugSphere(
                            GetWorld(),
                            CurrentWorldLocation,
                            5.f,
                            30,
                            FColor::Red,
                            true,
                            3.f
                        );

                        // Update the character's world location
                        Character->SetActorLocation(CurrentWorldLocation);
                        
                        // Log the new world location
                        //UE_LOG(LogTemp, Warning, TEXT("New World Location: %s"), *CurrentWorldLocation.ToString());

                        // Reset the mesh to its initial relative position and rotation
                        CharacterMesh->SetRelativeLocation(CharacterMeshLocalLocation);
                        CharacterMesh->SetRelativeRotation(CharacterMeshLocalRotation);

                        // Force update transform to ensure changes take effect immediately
                        USceneComponent* RootComp = Character->GetRootComponent();
                        if (RootComp)
                        {
                            RootComp->UpdateComponentToWorld();
                        }

                        // Re-enable movement if necessary
                        CharacterMesh->SetEnableGravity(true);
                        if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
                        {
                            MoveComp->GravityScale = 1.f;
                            MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
                        }
                    }
                    Character->SetHP(Character->GetHP() - Damage);
                    if (Character->IsDead())
                    {
                        Character->Die();
                    }
                }
            }
        }
    }
}
