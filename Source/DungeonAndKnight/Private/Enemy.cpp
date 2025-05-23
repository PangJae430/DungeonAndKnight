// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. mesh의 내용을 채우고 싶다
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
	tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux'"));
	if (tempMesh.Succeeded())
	{
		GetMesh() -> SetSkeletalMesh(tempMesh.Object);
		GetMesh() -> SetRelativeLocationAndRotation
		(FVector(0,0,-90.f), FRotator(0,-90.f,0));
	}
	//GetMesh() -> SetCollisionProfileName(TEXT(" "));
	EnemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

