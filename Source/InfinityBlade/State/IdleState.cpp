// Fill out your copyright notice in the Description page of Project Settings.

#include "IdleState.h"

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

}

void FIdleState::Start()
{
	FState::Start();

	CurState = EStateType::IDLE;
}

void FIdleState::Stop()
{
	FState::Stop();

}