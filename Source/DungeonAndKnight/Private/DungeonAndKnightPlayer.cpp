// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonAndKnightPlayer.h"

//#include "../DungeonAndKnight.h"
#include "AssetSelection.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADungeonAndKnightPlayer::ADungeonAndKnightPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. mesh의 내용을 채우고 싶다
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
	tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (tempMesh.Succeeded())
	{
		GetMesh() -> SetSkeletalMesh(tempMesh.Object);
		GetMesh() -> SetRelativeLocationAndRotation
		(FVector(0,0,-90.f), FRotator(0,-90.f,0));
	}
	

	ConstructorHelpers::FObjectFinder<UInputMappingContext>
	tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/DungeonAndKnight/Input/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
	{
		IMC_Player=tempIMC.Object;
	}

	// 칼을 생성하고 싶다
	SwordComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	SwordComp->SetupAttachment(GetMesh(),"Sword");
	SwordComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	

	ConstructorHelpers::FObjectFinder<UStaticMesh>
	tempSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/DungeonAndKnight/Assets/A_Models/sword/source/Practice_sword.Practice_sword'"));
	if (tempSwordMesh.Succeeded())
	{
		SwordComp->SetStaticMesh(tempSwordMesh.Object);
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 점프 높이 설정
	GetCharacterMovement()->JumpZVelocity = 600.f;
	//점프 중 공중에서 조작 가능 정도
	GetCharacterMovement()->AirControl = 0.75f;

	bIsAttack=false;
	
}

// Called when the game starts or when spawned
void ADungeonAndKnightPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADungeonAndKnightPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// 현재 컨트롤러가 플레이컨트롤러가 맞다면
	if (auto* pc = Cast<APlayerController>(Controller))
	{
		// UEnhancedInputLocalPlayerSubsystem를 가져와서
		auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(pc->GetLocalPlayer());
		
		// AddMappingContext를 하고싶다.
		subsystem -> RemoveMappingContext(IMC_Player);
		subsystem -> AddMappingContext(IMC_Player,0);
		
	}
}



// Called every frame
void ADungeonAndKnightPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ControlRotation값으로 Transform을 만들고 그것을 기준으로 방향을 변경하고
	// AddMovementInput 하고싶다.
	FTransform t = FTransform(GetControlRotation());
	Direction = t.TransformVector(Direction);
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void ADungeonAndKnightPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto*input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input -> BindAction(IA_PlayerMove,ETriggerEvent::Triggered,this,&ADungeonAndKnightPlayer::OnActionMove);
		input -> BindAction(IA_PlayerJump,ETriggerEvent::Started,this,&ADungeonAndKnightPlayer::OnActionJump);
		input -> BindAction(IA_PlayerAttack,ETriggerEvent::Started,this,&ADungeonAndKnightPlayer::OnActionAttackStart);
		input -> BindAction(IA_PlayerAttack,ETriggerEvent::Completed,this,&ADungeonAndKnightPlayer::OnActionAttackEnd);
	}

}

// 캐릭터 이동
void ADungeonAndKnightPlayer::OnActionMove(const FInputActionValue& value)
{
	if (!bIsAttack)
	{
	GetCharacterMovement()->RotationRate= FRotator(0.f,720.f,0.f);
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	}
	
		

	
}

//캐릭터 점프
void ADungeonAndKnightPlayer::OnActionJump(const FInputActionValue& value)
{
	Jump();
}

//공격시작
void ADungeonAndKnightPlayer::OnActionAttackStart(const FInputActionValue& value)
{
	if (bIsAttack==false)
	{
		bIsAttack=true;
		GetCharacterMovement()-> MaxWalkSpeed = 0.f;
	}

	
	FHitResult OutHit;
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 200.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bhit = GetWorld()-> LineTraceSingleByChannel(OutHit,start,end,ECC_Visibility,Params);
	
	// 라인트레이스 디버그라인(빨간줄)
	DrawDebugLine(GetWorld(), start,end,FColor::Red, false, 2.0f, 0, 1.0f);
	
	if (bhit==true)
	{
		if (AEnemy* Enemy =Cast<AEnemy>(OutHit.GetActor()))
		{
			Enemy -> EnemyFSM -> OnMyTakeDamage(1);
		}
	}
}

//공격 끝
void ADungeonAndKnightPlayer::OnActionAttackEnd(const FInputActionValue& value)
{
	bIsAttack=false;
	GetCharacterMovement()-> MaxWalkSpeed = 600.f;
}
