// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Blueprint.h"


// Sets default values
ASkill::ASkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(
			TEXT("StaticMesh'/Game/Blueprints/Skill/MaterialSphere_Collision.MaterialSphere_Collision'"));
		if (Asset.Succeeded())
		{
			StaticMesh->SetStaticMesh(Asset.Object);
		}
	}

	//test
	{
		static ConstructorHelpers::FObjectFinder<UBlueprint> Asset(
			TEXT("Blueprint'/Game/Blueprints/Skill/MySkill.MySkill'")
		);
		if (Asset.Succeeded())
		{
			BPAsset = Asset.Object;
		}
	}
	
	StaticMesh->CastShadow = false;
	StaticMesh->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.5);
	
}

// Called every frame
void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBlueprint* ASkill::GetBPAsset()
{
	return BPAsset;
}