// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "State/StateType.h"
#include "PlayerCharacter.generated.h"

class FState;

UCLASS()
class INFINITYBLADE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	int AttackPoint;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AttackRangeBox;

	UPROPERTY(EditAnywhere, Category = Materials, meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* RangeDecalMaterial;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//About attack
public:
	TArray<AActor*> CheckAttackRange();
	void SetAttackRange();
	void ResetAttackRange();
	int GetAttackPoint() const;

	UFUNCTION(BlueprintCallable)
	void FinishAttack();

	//State
protected:
	TMap<EStateType, FState*> StateMap;
	FState* CurState;
	EStateType CurStateType;

public:
	void InitState();
	void ChangeState(EStateType StateType);
	FString GetEStateAsString(EStateType EnumValue);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	EStateType GetCurStateType() const;

	FVector* GetTargetPosition();
	bool HasTargetPosition();
};
