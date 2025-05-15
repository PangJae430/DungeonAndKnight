// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Enemy.h"

void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Me =Cast<AEnemy>(TryGetPawnOwner());
}



void UEnemyAnim::PlayDamageAnimation()
{
	if (Me)
	{
		Me->PlayAnimMontage(HitMontage,1);
	}
}

void UEnemyAnim::PlayDieAnimation()
{
	if (Me)
	{
		Me->PlayAnimMontage(DieMontage,1,FName("Die"));
	}
}

void UEnemyAnim::AnimNotify_AttackEnd()
{
	if (Me)
	{
		bAttack=false;
	}
}

