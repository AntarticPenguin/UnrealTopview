// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadState.h"

FDeadState::FDeadState()
{
}

FDeadState::~FDeadState()
{
}

void FDeadState::Init(APlayerCharacter* InCharacter)
{
	FState::Init(InCharacter);
}

void FDeadState::Update(float DeltaTime)
{
	FState::Update(DeltaTime);
}

void FDeadState::Start()
{
	FState::Start();

	CurState = EStateType::DEAD;
	UE_LOG(LogClass, Warning, TEXT("State::%s Start"), *GetEStateAsString(CurState));
}

void FDeadState::Stop()
{
	FState::Stop();
}