// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

private:
	bool bIsMousePressed;

	FVector TargetLocation;
	bool bHasTargetLocation;
	float DistTolerance;

	FRotator TargetRotator;
	FRotator SmoothRotator;
	
protected:
	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;
	void MoveAndRotation(float DeltaTime);

	virtual void SetupInputComponent() override;

	void OnLeftMousePressed();
	void OnLeftMouseReleased();

	void SetTargetLocation(FVector InTargetLocation);
};
