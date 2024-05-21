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
        GetWorld()->GetTimerManager().SetTimer(AfterExplosionTimerHandle, this, &AGrenade::ReturnActorsToNormalState, 7.f, false);
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

	UE_LOG(LogTemp, Warning, TEXT("OverlappingFound is %s"), OverlappingFound ? TEXT("true") : TEXT("false"));

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
				Direction.Normalize();
				float ForceMagnitude = 30000.f;

				ACharacter* Character = Cast<ACharacter>(Pawn);
				if (Character)
				{
					if (Character->GetMesh())
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

							UE_LOG(LogTemp, Warning, TEXT("Original rotation of the character: %s"), *Rotation.ToString());
							UE_LOG(LogTemp, Warning, TEXT("Original location of the character: %s"), *Location.ToString());

							Character->GetMesh()->SetSimulatePhysics(true);
							ActorsInExplosionRadiusCount++;
						}
						
						Character->GetMesh()->AddForce(Direction * ForceMagnitude);
					}
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
				ACharacter* Character = Cast<ACharacter>(Pawn);
				if (Character)
				{
					if (Character->GetMesh())
					{
						Character->GetMesh()->SetSimulatePhysics(false);

						if (OriginalRotations.Contains(Character))
						{
							Character->SetActorRotation(OriginalRotations[Character]);
							UE_LOG(LogTemp, Warning, TEXT("set actor rotation back to %s"), *OriginalRotations[Character].ToString());
						}
						if (OriginalPositions.Contains(Character))
						{
							Character->SetActorLocation(OriginalPositions[Character]);
							UE_LOG(LogTemp, Warning, TEXT("set actor location back to %s"), *OriginalPositions[Character].ToString());
						}
					}
				}
			}	
		}
	}
}

