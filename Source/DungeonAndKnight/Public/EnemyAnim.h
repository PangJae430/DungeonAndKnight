// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
