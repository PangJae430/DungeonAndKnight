// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	Damage UMETA(DisplayName = "Damage"),
	Die UMETA(DisplayName = "Die"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONANDKNIGHT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		EEnemyState State;

	UPROPERTY()
	class ADungeonAndKnightPlayer* Player;

	UPROPERTY()
	class AEnemy* Me;

	//공격 사거리
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackRange = 200.f;

	//공격대기시간
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackDelayTime = 2.f;

	float CurrentTime;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	//플레이어가 나를 공격하면 데미지처리를 하고싶다.
	//공격받은 기능
	void OnMyTakeDamage(int32 damage);

	//최대체력과 현재체력
	float MaxHP = 2.f;
	float CurHP = 2.f;

	void SetState(EEnemyState next);

	float DieDownSpeed = 200.f;

	// 플레리어 감지 거리
	float PlayerDetectDist = 400.f;
	
	
};
