// Fill out your copyright notice in the Description page of Project Settings.

#include "State.h"
#include "Character/PlayerCharacter.h"
#include "UObject/UObjectGlobals.h"

FState::FState()
{
}

FState::~FState()
{
}

void FState::Init(APlayerCharacter* InCharacter)
{
	Character = InCharacter;
}

void FState::Update(float DeltaTime)
{
	if (EStateType::NONE != NextState)
	{
		Character->ChangeState(NextState);
		return;
	}
}

void FState::Start()
{
	NextState = EStateType::NONE;
	CurState = EStateType::NONE;
}

void FState::Stop()
{
	
}

FString FState::GetEStateAsString(EStateType EnumValue)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameStringByIndex((int32)EnumValue);
}