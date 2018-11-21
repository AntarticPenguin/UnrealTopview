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

	auto Controller = Cast<APlayerCharacterController>(Character->GetController());

	if (nullptr != Controller->GetTargetPosition())
	{
		if (FVector::Dist(Character->GetActorLocation(), *Controller->GetTargetPosition())
			< Controller->GetDistTolerance())
		{
			UE_LOG(LogClass, Warning, TEXT("Arrived"));
			Controller->ResetTargetPosition();
			Character->ChangeState(EStateType::IDLE);
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

}