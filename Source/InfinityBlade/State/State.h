// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateType.h"

/**
 * 
 */
class INFINITYBLADE_API FState
{
protected:
	class APlayerCharacter* Character;
	EStateType NextState;
	EStateType CurState;

public:
	FState();
	virtual ~FState();

public:
	virtual void Init(APlayerCharacter* InCharacter);
	virtual void Update(float DeltaTime);
	virtual void Start();
	virtual void Stop();

	FString GetEStateAsString(EStateType EnumValue);
};
