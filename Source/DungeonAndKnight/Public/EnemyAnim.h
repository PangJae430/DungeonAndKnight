// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONANDKNIGHT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeBeginPlay() override;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class AEnemy* Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState State;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	bool bAttack;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class UAnimMontage* HitMontage;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class UAnimMontage* DieMontage;

	void PlayDamageAnimation();
	
	void PlayDieAnimation();

	bool bIsDie = false;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DamagEnd();

	UFUNCTION()
	void AnimNotify_DieEnd();
	
	
};
