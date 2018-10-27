// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "PlayerCharacterController.h"
#include "Enemy/Enemy.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Materials/M_CursorDecal.M_CursorDecal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	//BoxCollision Settings
	AttackRangeBox = CreateDefaultSubobject<UBoxComponent>("AttackRangeBox");
	AttackRangeBox->bGenerateOverlapEvents = true;
	AttackRangeBox->SetupAttachment(RootComponent);
	AttackRangeBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackRangeBox->SetBoxExtent(FVector(64.0f, 64.0f, 64.0f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TArray<AActor*> APlayerCharacter::CheckAttackRange()
{
	//AttackRangeBox->OnComponentBeginOverlap.AddDynamic();
	
	SetAttackRange();
	
	TArray<AActor*> Enemies;
	AttackRangeBox->GetOverlappingActors(Enemies, AEnemy::StaticClass());
	
	ResetAttackRange();

	return Enemies;
}

void APlayerCharacter::SetAttackRange()
{
	FVector NewSize = FVector(80.0f, 64.0f, 64.0f);
	float BoxSize = AttackRangeBox->GetScaledBoxExtent().X;
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BoxSize;

	AttackRangeBox->SetBoxExtent(NewSize);
	AttackRangeBox->SetWorldLocation(NewLocation);
	UE_LOG(LogClass, Warning, TEXT("x: %f, y: %f, z: %f"), NewLocation.X, NewLocation.Y, NewLocation.Z);
}

void APlayerCharacter::ResetAttackRange()
{
	FVector NewSize = FVector(64.0f, 64.0f, 64.0f);
	AttackRangeBox->SetBoxExtent(NewSize);
	AttackRangeBox->SetRelativeLocation(FVector::ZeroVector);
}

int APlayerCharacter::GetAttackPoint()
{
	return AttackPoint;
}