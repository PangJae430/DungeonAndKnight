// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONANDKNIGHT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//UFUNCTION()
	//void AnimNotifyEnableCombo(UAnimNotify* notify);

	// UFUNCTION()
	// void AnimNotifyAttackEnd(UAnimNotify* notify);
	
	UPROPERTY()
	class ADungeonAndKnightPlayer* Player;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Direction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsInAir;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	

	
	
	
};
