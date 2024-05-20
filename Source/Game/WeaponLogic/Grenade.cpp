// Fill out your copyright notice in the Description page of Project Settings.


#include "./Grenade.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Character/DefaultMainCharacter.h"

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

}

bool AGrenade::Explode()
{
	if (this)
	{
		GetOverlappingActorsInRadiusOfExplosion();
		ApplyForceToOverlappingActors();
		Destroy();
	}
	return true;
}

bool AGrenade::GetOverlappingActorsInRadiusOfExplosion()
{
	ExplosionLocation = Sphere->GetComponentLocation();

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
			for (AActor* Actor : ActorsInExplosionRadius)
			{
				APawn* Pawn = Cast<APawn>(Actor);
				if (Pawn && Pawn->GetMovementComponent())
				{
					// UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Pawn->GetRootComponent());
					// if (PrimitiveComponent)
					// {
					// 	PrimitiveComponent->SetSimulatePhysics(true);
					// }

					// ACharacter* Character = Cast<ACharacter>(Pawn);
					// if (Character)
					// {
					// 	Character->GetMesh()->SetSimulatePhysics(true);
					// }



					// UCharacterMovementComponent* CharacterMovementComp = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
					
					// if (CharacterMovementComp)
					// {
					// 	FVector Direction = ExplosionLocation - Pawn->GetActorLocation();
					// 	Direction.Normalize();

					// 	float ForceMagnitude = 20000.f;
					// 	CharacterMovementComp->AddForce(Direction * ForceMagnitude);
					// 	UE_LOG(LogTemp, Warning, TEXT("Force added to: %s"), *Pawn->GetName());
					// }


					ADefaultMainCharacter* DefCharacter = Cast<ADefaultMainCharacter>(Pawn);
					if (DefCharacter)
					{
						DefCharacter->Die();
					}
					
				}
			}
	}
	return true;
}
