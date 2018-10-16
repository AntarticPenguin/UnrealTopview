// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacterController::APlayerCharacterController()
{
}

void APlayerCharacterController::BeginPlay()
{
	bShowMouseCursor = true;
	bIsMousePressed = false;
	bHasTargetLocation = false;
	DistTolerance = 110;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	ACharacter* Character = GetCharacter();

	if (bHasTargetLocation)
	{
		if(FVector::Dist(Character->GetActorLocation(), TargetLocation) < DistTolerance)
		{
			bHasTargetLocation = false;
			return;
		}

		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		float Speed = MovementComponent->MaxWalkSpeed;
		FVector Direction = (TargetLocation - Character->GetActorLocation()).GetSafeNormal();
		
		MovementComponent->MoveSmooth(Direction * Speed, DeltaTime);
	}

	if (bIsMousePressed)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			if (Character)
			{
				SetTargetLocation(Hit.ImpactPoint);
			}
		}
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &APlayerCharacterController::OnLeftMousePressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &APlayerCharacterController::OnLeftMouseReleased);
}

void APlayerCharacterController::OnLeftMousePressed()
{
	bIsMousePressed = true;
}

void APlayerCharacterController::OnLeftMouseReleased()
{
	bIsMousePressed = false;
}

void APlayerCharacterController::SetTargetLocation(FVector InTargetLocation)
{
	TargetLocation = InTargetLocation;
	bHasTargetLocation = true;
	
	float Distance = FVector::Dist(GetCharacter()->GetActorLocation(), TargetLocation);
	UE_LOG(LogClass, Warning, TEXT("Distance: %f"), Distance);
}