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
		Me->PlayAnimMontage(HitMontage,1,TEXT("Damage"));
	}
}

void UEnemyAnim::PlayDieAnimation()
{
	if (Me)
	{
		Montage_Play(DieMontage,1);
		
	}
}

void UEnemyAnim::AnimNotify_AttackEnd()
{
	if (Me)
	{
		bAttack=false;
	}
}
 
void UEnemyAnim::AnimNotify_DamagEnd()
{
	if (Me && HitMontage)
	{
		Montage_Stop(0.25f,HitMontage);
		Me-> EnemyFSM->SetState(EEnemyState::Move);
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	bIsDie=true;
}

