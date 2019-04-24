// Fill out your copyright notice in the Description page of Project Settings.

#include "IdleState.h"
#include "Character/PlayerCharacter.h"

FIdleState::FIdleState()
{
}

FIdleState::~FIdleState()
{
}

void FIdleState::Init(APlayerCharacter* InCharacter)
{
	FState::Init(InCharacter);

}

void FIdleState::Update(float DeltaTime)
{
	FState::Update(DeltaTime);

	if (Character->HasTargetPosition())
	{
		NextState = EStateType::MOVE;
	}

}

void FIdleState::Start()
{
	FState::Start();

	CurState = EStateType::IDLE;
	UE_LOG(LogClass, Warning, TEXT("State::%s Start"), *GetEStateAsString(CurState));
}

void FIdleState::Stop()
{
	FState::Stop();

}