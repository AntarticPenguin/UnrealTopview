// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackState.h"
#include "Character/PlayerCharacter.h"
#include "Character/PlayerCharacterController.h"
#include "Enemy/Enemy.h"

FAttackState::FAttackState()
{
}

FAttackState::~FAttackState()
{
}

void FAttackState::Init(APlayerCharacter* InCharacter)
{
	FState::Init(InCharacter);

}

void FAttackState::Update(float DeltaTime)
{
	FState::Update(DeltaTime);

}

void FAttackState::Start()
{
	FState::Start();
	CurState = EStateType::ATTACK;
	UE_LOG(LogClass, Warning, TEXT("State::%s Start"), *GetEStateAsString(CurState));
	
	if (Character)
	{
		TArray<AActor*> Enemies = Character->CheckAttackRange();
		for (auto Enemy : Enemies)
		{
			UE_LOG(LogClass, Warning, TEXT("Attack to %s, Dmg: %d"), *(Enemy->GetName()), Character->GetAttackPoint());
			Cast<AEnemy>(Enemy)->DecreaseHp(Character->GetAttackPoint());
		}
	}
}

void FAttackState::Stop()
{
	FState::Stop();

}