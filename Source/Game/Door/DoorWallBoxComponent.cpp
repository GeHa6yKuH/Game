// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWallBoxComponent.h"
#include "MoveDown.h"
#include "../Character/DefaultMainCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

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
        ADefaultMainCharacter* MainCharacter = Cast<ADefaultMainCharacter>(Actor);
        MainCharacter->PlayAnimMontage(CardMontage);
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
            ADefaultMainCharacter* MainCharacter = Cast<ADefaultMainCharacter>(Actor);
            if (MainCharacter)
            {
                if (APlayerController* PlayerController = MainCharacter->GetController<APlayerController>())
                {
                    IsPlayerInBox = true; // Player is in the box

                    // Handle open widget
                    if (MoveDownComp && !OpenWidgetAdded && MoveDownComp->IsDoorClosed())
                    {
                        DoorWidgetScreen = CreateWidget(MainCharacter->GetController<APlayerController>(), DoorInteractWidgetClass);
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
                        DoorCloseWidgetScreen = CreateWidget(MainCharacter->GetController<APlayerController>(), DoorInteractCloseWidgetClass);
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
                    if (PlayerController->IsInputKeyDown(FKey("F")))
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
