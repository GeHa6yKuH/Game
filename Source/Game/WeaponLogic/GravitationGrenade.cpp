// Fill out your copyright notice in the Description page of Project Settings.


#include "./GravitationGrenade.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Character/DefaultMainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"

void AGravitationGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsExploding)
	{
		ApplyForceToOverlappingActors();
	}
}

bool AGravitationGrenade::ApplyForceToOverlappingActors()
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
                USkeletalMeshComponent* CharacterMesh = Character->GetMesh();

                if (Character && CharacterMesh)
                {
                    if (ActorsInExplosionRadiusCount < ActorsInExplosionRadiusSize)
                    {
                        UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance();
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

                        CharacterMesh->SetEnableGravity(false);
                        if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
                        {
                            MoveComp->GravityScale = 0.f;
                        }

                        CharacterMesh->SetSimulatePhysics(true);
                        ActorsInExplosionRadiusCount++;
                    }

                    // Ensure the character stays within the explosion radius
                    FVector NewLocation = CharacterMesh->GetRelativeLocation();
                    FVector ToExplosionCenter = NewLocation - (CharacterMeshLocalLocation + Direction);
                    if (ToExplosionCenter.Size() > ExplosionRadius)
                    {
                        // FVector CounterForce = -ToExplosionCenter.GetSafeNormal() * ForceMagnitude * DistanceFactor;
                        CharacterMesh->AddForce(-Force);
                        return true;
                    }

                    // Apply the force
                    CharacterMesh->AddForce(Force);
                }
            }
        }
    }
    return true;
}
