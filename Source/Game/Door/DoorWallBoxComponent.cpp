// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWallBoxComponent.h"
#include "MoveDown.h"
#include "../Character/DefaultMainCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

UDoorWallBoxComponent::UDoorWallBoxComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UDoorWallBoxComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDoorWallBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Actor = GetAcceptableActor();
    if (Actor)
    {
        MainCharacter = Cast<ADefaultMainCharacter>(Actor);
        if (MainCharacter)
        {
            PlayerController = Cast<APlayerController>(MainCharacter->GetController());
            MainCharacter->DisableInput(PlayerController);
            MainCharacter->SetIsPlayingAnimDoorTrue();
            MainCharacter->HideWeapon();
            // spawning card into player hands
            MainCharacter->SpawnCardInHandsBeforeAnim();
            //playing animation
            MainCharacter->PlayAnimMontage(CardMontage);

            FTimerHandle OpenOrCloseTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(OpenOrCloseTimerHandle, this, &UDoorWallBoxComponent::CloseOrOpenTheDoor, 2.9f, false);

            FTimerHandle InputHandlingTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(InputHandlingTimerHandle, this, &UDoorWallBoxComponent::EnablePhysicsOnCapturedActor, 2.9f, false);

            // removing card after anim with delay
            FTimerHandle CardRemovingTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(CardRemovingTimerHandle, MainCharacter, &ADefaultMainCharacter::RemoveCardFromHandsAfterAnim, 2.9f, false);
        }
    }
}

void UDoorWallBoxComponent::EnablePhysicsOnCapturedActor()
{
    if (MainCharacter && PlayerController)
    {
        MainCharacter->EnableInput(PlayerController);
    }
    
}

void UDoorWallBoxComponent::CloseOrOpenTheDoor()
{
    if (MoveDownComp->IsDoorClosed())
		{
			MoveDownComp->SetShouldDoorMoveDown(false);
			MoveDownComp->SetShouldDoorMove(true);
		} else if (MoveDownComp->IsDoorOpen())
		{
			MoveDownComp->SetShouldDoorMove(false);
			MoveDownComp->SetShouldDoorMoveDown(true);
		}
}

AActor* UDoorWallBoxComponent::GetAcceptableActor()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	IsPlayerInBox = false; // Reset for this tick

	for (AActor* Actor : Actors)
    {
        if (Actor && Actor->IsA(ADefaultMainCharacter::StaticClass()))
        {
            ADefaultMainCharacter* MainCharacterActor = Cast<ADefaultMainCharacter>(Actor);
            if (MainCharacterActor)
            {
                if (APlayerController* ActorPlayerController = MainCharacterActor->GetController<APlayerController>())
                {
                    IsPlayerInBox = true; // Player is in the box

                    // Handle open widget
                    if (MoveDownComp && !OpenWidgetAdded && MoveDownComp->IsDoorClosed())
                    {
                        DoorWidgetScreen = CreateWidget(ActorPlayerController, DoorInteractWidgetClass);
                        if (DoorWidgetScreen)
                        {
                            DoorWidgetScreen->AddToViewport();
                            OpenWidgetAdded = true;
                        }
                    }
                    else if (DoorWidgetScreen && OpenWidgetAdded && MoveDownComp && MoveDownComp->GetShouldDoorMove())
                    {
						DoorWidgetScreen->RemoveFromParent();
                        OpenWidgetAdded = false;
                    }

                    // Handle close widget
                    if (MoveDownComp && !CloseWidgetAdded && MoveDownComp->IsDoorOpen())
                    {
                        DoorCloseWidgetScreen = CreateWidget(ActorPlayerController, DoorInteractCloseWidgetClass);
                        if (DoorCloseWidgetScreen)
                        {
                            DoorCloseWidgetScreen->AddToViewport();
                            CloseWidgetAdded = true;
                        }
                    }
                    else if (DoorCloseWidgetScreen && CloseWidgetAdded && MoveDownComp && MoveDownComp->GetShouldDoorMoveDown())
                    {
						DoorCloseWidgetScreen->RemoveFromParent();
                        CloseWidgetAdded = false;
                    }

                    // Check for key press
                    if (ActorPlayerController->IsInputKeyDown(FKey("F")))
                    {
                        return Actor;
                    }
                }
            }
        }
    }

	// Handle player leaving the box
	if (!IsPlayerInBox)
	{
		if (OpenWidgetAdded)
		{
			DoorWidgetScreen->RemoveFromParent();
			OpenWidgetAdded = false;
		}
		if (CloseWidgetAdded)
		{
			DoorCloseWidgetScreen->RemoveFromParent();
			CloseWidgetAdded = false;
		}
	}

	return nullptr;
}


void UDoorWallBoxComponent::SetMoveDownComp(class UMoveDown* NewMoveDownComp)
{
	MoveDownComp = NewMoveDownComp;
}
