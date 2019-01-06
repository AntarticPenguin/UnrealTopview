// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "StateType.generated.h"

UENUM(BlueprintType, Category = "StateType")
enum class EStateType : uint8
{
	NONE,
	IDLE,
	MOVE,
	ATTACK,
	DEAD,
};