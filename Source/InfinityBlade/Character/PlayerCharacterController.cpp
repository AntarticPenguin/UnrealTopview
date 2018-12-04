// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "Enemy/Enemy.h"

APlayerCharacterController::APlayerCharacterController()
{
}

void APlayerCharacterController::BeginPlay()
{
	bShowMouseCursor = true;
	bIsMousePressed = false;
	TargetPosition = nullptr;
	DistTolerance = 110;
	
	Character = Cast<APlayerCharacter>(GetCharacter());
	if (!Character)
	{
		UE_LOG(LogClass, Warning, TEXT("PlayerCharacterController: Failed Get Character!!"));
	}
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bIsMousePressed)
	{
		switch (Character->GetCurStateType())
		{
		case EStateType::IDLE:
			WaitSetTargetPosition();
			break;
		case EStateType::MOVE:
			WaitSetTargetPosition();
			break;
		case EStateType::ATTACK:
			break;
		default:
			break;
		}
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &APlayerCharacterController::OnLeftMousePressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &APlayerCharacterController::OnLeftMouseReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &APlayerCharacterController::OnRightMousePressed);
	InputComponent->BindAction("RightMouse", IE_Released, this, &APlayerCharacterController::OnRightMouseReleased);

}

void APlayerCharacterController::OnLeftMousePressed()
{
	bIsMousePressed = true;
}

void APlayerCharacterController::OnLeftMouseReleased()
{
	bIsMousePressed = false;
}

void APlayerCharacterController::OnRightMousePressed()
{
	if (EStateType::ATTACK != Character->GetCurStateType())
	{
		Character->ChangeState(EStateType::ATTACK);
	}
}

void APlayerCharacterController::OnRightMouseReleased()
{

}

void APlayerCharacterController::MoveAndRotation(float DeltaTime)
{
	ACharacter* Character = GetCharacter();

	//Smooth Move
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	float Speed = MovementComponent->MaxWalkSpeed;
	FVector Direction = (*TargetPosition - Character->GetActorLocation()).GetSafeNormal();
	MovementComponent->MoveSmooth(Direction * Speed, DeltaTime);

	//Smooth Rotation
	SmoothRotator = FMath::RInterpTo(SmoothRotator, Direction.Rotation(), DeltaTime, SmoothTargetViewRotationSpeed);
	SetControlRotation(SmoothRotator);
}

void APlayerCharacterController::SetTargetPosition(FVector InTargetPosition)
{
	TargetPosition = new FVector(InTargetPosition);

	SmoothRotator = GetCharacter()->GetActorRotation();
}

FVector* APlayerCharacterController::GetTargetPosition() const
{
	return TargetPosition;
}

float APlayerCharacterController::GetDistTolerance() const
{
	return DistTolerance;
}

void APlayerCharacterController::ResetTargetPosition()
{
	TargetPosition = nullptr;
}

void APlayerCharacterController::WaitSetTargetPosition()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetTargetPosition(Hit.ImpactPoint);
	}
}