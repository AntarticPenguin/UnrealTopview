// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

UCLASS()
class INFINITYBLADE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

private:
	class APlayerCharacter* Character;
	bool bIsMousePressed;

	FVector* TargetPosition;
	float DistTolerance;

	FRotator SmoothRotator;
	
protected:
	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void OnLeftMousePressed();
	void OnLeftMouseReleased();
	void OnRightMousePressed();
	void OnRightMouseReleased();

public:
	void MoveAndRotation(float DeltaTime);

	void SetTargetPosition(FVector InTargetPosition);
	FVector* GetTargetPosition() const;
	float GetDistTolerance() const;

	void ResetTargetPosition();

	//Function related character' state
public:
	void WaitSetTargetPosition();
};
