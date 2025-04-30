// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "DungeonAndKnightPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Player = Cast<ADungeonAndKnightPlayer>(TryGetPawnOwner());
	
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//폴링
	// 1. 오너가 유효하지 않으면 종료한다
	if (nullptr == Player || false == Player ->IsValidLowLevel())
		return;

	//2. Speed 의 값을 채우고 싶다
	Speed = FVector::DotProduct( Player ->GetVelocity(), Player -> GetActorForwardVector());

	bIsInAir =Player->GetCharacterMovement()->IsFalling();
	
}
