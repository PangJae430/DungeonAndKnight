// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCamera.h"

#include "DungeonAndKnightPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCamera::APlayerCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp ->SetupAttachment(RootComponent);
	
	// 기본 카메라 오프셋 설정 (x: -500, z: 900)
	CameraOffset = FVector(-600.f, 0.f, 200.f);
	CameraComp->SetRelativeLocation(CameraOffset);
	CameraComp-> FieldOfView = 75.f;
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어를 찾고 카메라 타겟으로 설정
	auto* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	FVector CameraLocation = PlayerCharacter->GetActorLocation() + CameraOffset;
	SetActorLocation(CameraLocation);  // 카메라의 위치 설정

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this);
	}
	
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto* player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (player)
	{
		FVector Location = player->GetActorLocation();
		
	}
	
	UpdateCameraPosition();

}

void APlayerCamera::UpdateCameraPosition()
{
	
	ADungeonAndKnightPlayer* Player = Cast<ADungeonAndKnightPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player)
	{
		// 플레이어의 위치를 얻고 Y축만 반영하여 카메라 위치 계산
		FVector TargetLocation = Player->GetActorLocation();
		FVector NewCameraLocation = TargetLocation + CameraOffset;
		NewCameraLocation.X = CameraOffset.X;  // X축 고정
		NewCameraLocation.Z = CameraOffset.Z;  // Z축 고정

		// 카메라 위치 설정
		SetActorLocation(NewCameraLocation);

		FVector Direction = TargetLocation - NewCameraLocation;
		FRotator NewRotation = Direction.Rotation();

		NewRotation.Pitch = -30.f;
		SetActorRotation(NewRotation);
	}
}

