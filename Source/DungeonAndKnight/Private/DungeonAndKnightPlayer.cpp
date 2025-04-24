// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonAndKnightPlayer.h"

//#include "../DungeonAndKnight.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	// 2. SpringArmComp를 만들어서 Root에 붙이고 배치하고 싶다
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp ->SetupAttachment(RootComponent);
	SpringArmComp ->TargetArmLength = 700.f;
	SpringArmComp ->SetWorldRotation(FRotator(-30.f,90.f,0.f));
	

	// 3. CameraComp를 만들어서 SpringArmComp에 붙이고 싶다
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp -> SetupAttachment(SpringArmComp);


	ConstructorHelpers::FObjectFinder<UInputMappingContext>
	tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/DungeonAndKnight/Input/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
	{
		IMC_Player=tempIMC.Object;
	}

	bUseControllerRotationYaw = false;
	SpringArmComp->bUsePawnControlRotation =true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 점프 높이 설정
	GetCharacterMovement()->JumpZVelocity = 600.f;
	//점프 중 공중에서 조작 가능 정도
	GetCharacterMovement()->AirControl = 0.6f;

	
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
		input -> BindAction(IA_PlayerAttack,ETriggerEvent::Started,this,&ADungeonAndKnightPlayer::OnActionAttack);
	}

}


void ADungeonAndKnightPlayer::OnActionMove(const FInputActionValue& value)
{
	GetCharacterMovement()->RotationRate= FRotator(0.f,720.f,0.f);
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	
}

void ADungeonAndKnightPlayer::OnActionJump(const FInputActionValue& value)
{
	Jump();
}

void ADungeonAndKnightPlayer::OnActionAttack(const FInputActionValue& value)
{
	
}

