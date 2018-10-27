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

		MoveAndRotation(DeltaTime);
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

void APlayerCharacterController::MoveAndRotation(float DeltaTime)
{
	ACharacter* Character = GetCharacter();

	//Smooth Move
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	float Speed = MovementComponent->MaxWalkSpeed;
	FVector Direction = (TargetLocation - Character->GetActorLocation()).GetSafeNormal();
	MovementComponent->MoveSmooth(Direction * Speed, DeltaTime);

	//Smooth Rotation
	SmoothRotator = FMath::RInterpTo(SmoothRotator, Direction.Rotation(), DeltaTime, SmoothTargetViewRotationSpeed);
	SetControlRotation(SmoothRotator);
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
	UE_LOG(LogClass, Warning, TEXT("Attack Button Clicked!!"));

	auto Character = Cast<APlayerCharacter>(GetCharacter());
	if (Character)
	{
		/*
			1. 공격범위 안에 적이 있는지 확인한다.
			2. 들어온 적에게 데미지를 준다.
		*/
		TArray<AActor*> Enemies = Character->CheckAttackRange();
		for (auto Enemy : Enemies)
		{
			UE_LOG(LogClass, Warning, TEXT("Name: %s"), *(Enemy->GetName()));
			Cast<AEnemy>(Enemy)->DecreaseHp(Character->GetAttackPoint());
		}
	}
}

void APlayerCharacterController::OnRightMouseReleased()
{

}

void APlayerCharacterController::SetTargetLocation(FVector InTargetLocation)
{
	TargetLocation = InTargetLocation;
	bHasTargetLocation = true;

	SmoothRotator = GetCharacter()->GetActorRotation();
}