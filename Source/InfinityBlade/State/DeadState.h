// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class INFINITYBLADE_API FDeadState : public FState
{
public:
	FDeadState();
	~FDeadState();

public:
	virtual void Init(APlayerCharacter* InCharacter) override;
	virtual void Update(float DeltaTime) override;
	virtual void Start() override;
	virtual void Stop() override;
};
