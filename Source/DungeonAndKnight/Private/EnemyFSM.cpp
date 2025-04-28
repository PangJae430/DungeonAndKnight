// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "DungeonAndKnightPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Me= Cast<AEnemy>(GetOwner());

	Me -> GetCharacterMovement()->bOrientRotationToMovement=true;
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	 case EEnemyState::Idle:
		TickIdle();
		break;

	 case EEnemyState::Move:
		TickMove();
		break;

	case EEnemyState::Attack:
		TickAttack();
		break;

	case EEnemyState::Damage:
		TickDamage();
		break;

	case EEnemyState::Die:
		TickDie();
		break;
	}
	FString log = UEnum::GetValueAsString(State);
	//PRINT_LOG(TEXT("%s"), *log);
	DrawDebugString(GetWorld(), Me->GetActorLocation(), log, nullptr, FColor::Yellow, 0, true, 1);
}

void UEnemyFSM::TickIdle()
{
	// 주인공을 찾아서 기억하고 싶다
	auto* c = GetWorld()->GetFirstPlayerController()->GetCharacter();
	Player = Cast<ADungeonAndKnightPlayer>(c);

	// 만약 플레이어를 기억했으면
	if (Player)
	 {
		//플레이어와 나와의 거리 계산
		FVector playerLoc = Player->GetActorLocation();
		FVector CurDetectDist = playerLoc - Me->GetActorLocation();
		float detectDist = CurDetectDist.Length();
		
		//만약 거리가 특정 값 이내라면
		if (detectDist <= PlayerDetectDist)
		{
			//이동상태로 전이하고 싶다
		 SetState(EEnemyState::Move);
		}
	 }
}

void UEnemyFSM::TickMove()
{
	//플레이어의 위치를 알고 싶다
	FVector destination = Player->GetActorLocation();
	//플레이어의 방향을 알고 싶다
	FVector dir = destination - Me->GetActorLocation();
	//그 방향으로 이동하고싶다
	Me-> AddMovementInput(dir.GetSafeNormal2D());

	//만약 공격 가능한 거리라면
	float dist = dir.Length();
	if (dist < AttackRange)
	{
		//공격상태로 전이하고싶다
		SetState(EEnemyState::Attack);
	}
}

void UEnemyFSM::TickAttack()
{
	//시간이 흐르다가
	CurrentTime += GetWorld()->GetDeltaSeconds();
	//만약 현지시간이 공격 대기시간을 초과하면
	if (CurrentTime > AttackDelayTime)
	{
		CurrentTime = 0.0f;
		FVector dir = Player->GetActorLocation() - Me->GetActorLocation();
	//만약 상대가 공격 가능한다면
		if (dir.Length() < AttackRange)
		{
			//로그출력 PRINT_LOG(TEXT("Attack!!!"))
			//PRINT_LOG(TEXT("Attack"));
		}
		//그렇지 않다면
		else
		{
			//이동상태로 전이하고싶다
			SetState(EEnemyState::Move);
		}
		
	}
}

void UEnemyFSM::TickDamage()
{
	//1초동안 멈췄다가 이동상태로 전이 되고싶다
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if (CurrentTime>1)
	{
		SetState(EEnemyState::Damage);
	}
	
}

void UEnemyFSM::TickDie()
{
	//1초 동안 아래로 이동하고 싶다
	CurrentTime+=GetWorld()->GetDeltaSeconds();
	FVector dir(0,0,-1);
	FVector newLoc = Me ->GetActorLocation() + dir * DieDownSpeed *GetWorld()->GetDeltaSeconds();
	Me->SetActorLocation(newLoc);
	
	if (CurrentTime>1)
	{
		//1초가 지난 후 파괴되고 싶다
		Me->Destroy();
	}
	
}

void UEnemyFSM::OnMyTakeDamage(int32 damage)
{
	//체력을 damage만큼 감소시키고 싶다
	CurHP = FMath::Max(0, CurHP-damage);
	// 만약 체력이 0보다 크다면 Damage상태로 전이하고 싶다
	if(CurHP>0)
	{
		SetState(EEnemyState::Damage);
	}
	// 그렇지 않다면 죽음 상태로 전이하고 싶다
	else
	{
		SetState(EEnemyState::Die);
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	State = next;
	CurrentTime = 0.f;
}

