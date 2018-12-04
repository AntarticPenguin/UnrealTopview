// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveState.h"
#include "Character/PlayerCharacter.h"
#include "Character/PlayerCharacterController.h"

FMoveState::FMoveState()
{
}

FMoveState::~FMoveState()
{
}

void FMoveState::Init(APlayerCharacter* InCharacter)
{
	FState::Init(InCharacter);

}

void FMoveState::Update(float DeltaTime)
{
	FState::Update(DeltaTime);

	if (Character->HasTargetPosition())
	{
		Destination = Character->GetTargetPosition();

		auto Controller = Cast<APlayerCharacterController>(Character->GetController());

		if (FVector::Dist(Character->GetActorLocation(), *Destination) < Controller->GetDistTolerance())
		{
			Controller->ResetTargetPosition();
			NextState = EStateType::IDLE;
			return;
		}

		Controller->MoveAndRotation(DeltaTime);
	}
}

void FMoveState::Start()
{
	FState::Start();

	CurState = EStateType::MOVE;
}

void FMoveState::Stop()
{
	FState::Stop();

	auto Controller = Cast<APlayerCharacterController>(Character->GetController());
	Controller->ResetTargetPosition();
}