// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DoorWallBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UDoorWallBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

private:

	class UMoveDown* MoveDownComp;

	AActor* GetAcceptableActor();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> DoorInteractWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> DoorInteractCloseWidgetClass;

	UPROPERTY()
	class UUserWidget* DoorWidgetScreen;

	UPROPERTY()
	class UUserWidget* DoorCloseWidgetScreen;


	UPROPERTY(EditAnywhere)
	bool OpenWidgetAdded = false;

	UPROPERTY(EditAnywhere)
	bool CloseWidgetAdded = false;

	UPROPERTY(VisibleAnywhere)
	bool IsPlayerInBox = false;

	UPROPERTY(VisibleAnywhere)
	int PlayerInBoxCount = 0;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage * CardMontage;

	class APlayerController* PlayerController;

	class ADefaultMainCharacter* MainCharacter;

public:

	UDoorWallBoxComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMoveDownComp(class UMoveDown* NewMoveDownComp);

	UFUNCTION(BlueprintCallable)
	void CloseOrOpenTheDoor();

	UFUNCTION()
	void EnablePhysicsOnCapturedActor();

};
