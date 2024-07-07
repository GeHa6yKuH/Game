// Fill out your copyright notice in the Description page of Project Settings.


#include "./GravitationalPushGrenade.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../Character/DefaultMainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"

AGravitationalPushGrenade::AGravitationalPushGrenade()
{
    PrimaryActorTick.bCanEverTick = true;

    ForceMagnitude = 30000.f;
}

bool AGravitationalPushGrenade::ApplyForceToOverlappingActors()
{
    if (!ActorsInExplosionRadius.IsEmpty())
    {
        ActorsInExplosionRadiusSize = ActorsInExplosionRadius.Num();
        for (AActor* Actor : ActorsInExplosionRadius)
        {
            APawn* Pawn = Cast<APawn>(Actor);
            if (Pawn)
            {
                FVector Direction = Pawn->GetActorLocation() - ExplosionLocation;
                float Distance = Direction.Size();

                if (Distance < MagnitudeIrrelatableDistance)
                {
                    continue;
                }

                Direction.Normalize();

                double ValueToClamp = Distance / 1000;
                float DistanceFactor = FMath::Clamp(ValueToClamp, 0.0f, 1.0f);
                FVector Force = Direction * ForceMagnitude / DistanceFactor;

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

                        Character->GetMesh()->SetSimulatePhysics(true);
                        ActorsInExplosionRadiusCount++;
                    }

                    // Apply the force
                    Character->GetMesh()->AddForce(Force);
                    UE_LOG(LogTemp, Warning, TEXT("second grenade works!"));
                }
            }
        }
    }
    return true;
}
