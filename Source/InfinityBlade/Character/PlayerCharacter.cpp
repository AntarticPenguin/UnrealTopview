// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "PlayerCharacterController.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"

#include "State/State.h"
#include "State/IdleState.h"
#include "State/MoveState.h"
#include "State/AttackState.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set Cursor Decal
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Materials/M_CursorDecal.M_CursorDecal'"));
		if (DecalMaterialAsset.Succeeded())
		{
			CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
		}
		CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
		CursorToWorld->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	}

	//Load RangeDecal Material
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Materials/M_RangeDecal.M_RangeDecal'"));
		if (DecalMaterialAsset.Succeeded())
		{
			RangeDecalMaterial = DecalMaterialAsset.Object;
		}
	}

	//BoxCollision Settings
	AttackRangeBox = CreateDefaultSubobject<UBoxComponent>("AttackRangeBox");
	AttackRangeBox->bGenerateOverlapEvents = true;
	AttackRangeBox->bReceivesDecals = false;
	AttackRangeBox->SetupAttachment(RootComponent);
	AttackRangeBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackRangeBox->SetBoxExtent(FVector(64.0f, 64.0f, 64.0f));
	AttackRangeBox->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitState();
	ChangeState(EStateType::IDLE);

	AttackPoint = 10;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != CursorToWorld)
	{
		if (APlayerCharacterController* Controller = Cast<APlayerCharacterController>(GetController()))
		{
			FHitResult Hit;
			Controller->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
			FVector CursorVector = Hit.ImpactNormal;
			FRotator CursorRotator = CursorVector.Rotation();
			CursorToWorld->SetWorldLocation(Hit.Location);
			CursorToWorld->SetWorldRotation(CursorRotator);
		}
	}

	CurState->Update(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TArray<AActor*> APlayerCharacter::CheckAttackRange()
{
	SetAttackRange();
	
	TArray<AActor*> Enemies;
	AttackRangeBox->GetOverlappingActors(Enemies, AEnemy::StaticClass());
	
	ResetAttackRange();

	return Enemies;
}

void APlayerCharacter::SetAttackRange()
{
	FVector NewSize = FVector(64.0f, 80.0f, 64.0f);
	float BoxSize = AttackRangeBox->GetScaledBoxExtent().Y;
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BoxSize;

	AttackRangeBox->SetBoxExtent(NewSize);
	AttackRangeBox->SetWorldLocation(NewLocation);
	
	//Draw RangeDecal
	NewSize.Z = 100.0f;
	FRotator NewRotator = AttackRangeBox->GetComponentRotation();
	auto RangeDecal = UGameplayStatics::SpawnDecalAtLocation(this, RangeDecalMaterial, NewSize, NewLocation, NewRotator, 0.0f);
	RangeDecal->SetFadeOut(0.0f, 1.0f);
}

void APlayerCharacter::ResetAttackRange()
{
	FVector NewSize = FVector(64.0f, 64.0f, 64.0f);
	AttackRangeBox->SetBoxExtent(NewSize);
	AttackRangeBox->SetRelativeLocation(FVector::ZeroVector);
}

int APlayerCharacter::GetAttackPoint() const
{
	return AttackPoint;
}

void APlayerCharacter::FinishAttack()
{
	ChangeState(EStateType::IDLE);
}

void APlayerCharacter::InitState()
{
	FState* IdleState = new FIdleState();
	FState* MoveState = new FMoveState();
	FState* AttackState = new FAttackState();

	IdleState->Init(this);
	MoveState->Init(this);
	AttackState->Init(this);

	StateMap.Add(EStateType::IDLE, IdleState);
	StateMap.Add(EStateType::MOVE, MoveState);
	StateMap.Add(EStateType::ATTACK, AttackState);
}

void APlayerCharacter::ChangeState(EStateType StateType)
{
	if (CurStateType == StateType)
		return;

	if (NULL != CurState)
	{
		CurState->Stop();
	}

	CurState = StateMap[StateType];
	CurStateType = StateType;

	CurState->Start();
}

EStateType APlayerCharacter::GetCurStateType() const
{
	return CurStateType;
}

FVector* APlayerCharacter::GetTargetPosition()
{
	return Cast<APlayerCharacterController>(GetController())->GetTargetPosition();
}

bool APlayerCharacter::HasTargetPosition()
{
	if (nullptr != GetTargetPosition())
		return true;
	return false;
}