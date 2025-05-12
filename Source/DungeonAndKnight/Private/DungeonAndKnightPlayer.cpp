// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonAndKnightPlayer.h"

//#include "../DungeonAndKnight.h"
#include "AssetSelection.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerAnim.h"
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

	// 공격상태 콤보 초기화
	bIsAttack=false;
	bCanNextCombo = false;
	bNextComboQueued = false;
	CurrentCombo = 0;
	MaxCombo = 4;
	
}

// Called when the game starts or when spawned
void ADungeonAndKnightPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto* pAnimInst = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (pAnimInst)
	{
		pAnimInst -> OnPlayMontageNotifyBegin.AddDynamic(this, &ADungeonAndKnightPlayer::HandleOnMontageNotifyBegin);
	}
	
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

//----------------------------------------------------------------------------------------------------
	
	FVector startLoc = SwordComp ->GetSocketLocation(TEXT("Start"));
	FVector endLoc = SwordComp ->GetSocketLocation(TEXT("End"));
	//int Steps = FMath::CeilToInt((endLoc-startLoc).Size()/15.f);
	FQuat Caprot = FRotationMatrix::MakeFromZ(endLoc-startLoc).ToQuat();

	if (bIsAttack)
		{
		// for (int32 i = 0; i <= Steps; i++)
		// {
		// 	float Alpha = i / static_cast<float>(Steps);
		// 	FVector Point = FMath::Lerp(startLoc, endLoc,0.f);

			TArray<FHitResult>OutHit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);


			bool bHit = GetWorld()-> SweepMultiByChannel(OutHit,startLoc,endLoc,FQuat::Identity,
				ECC_Pawn,FCollisionShape::MakeCapsule(15.f,80.f),Params);
			
			if (bHit==true)
			{
				for(const FHitResult& Hit: OutHit)
				{
					AActor* hitActor = Hit.GetActor();
					if (hitActor && !DamagedActorThisAttack.Contains(hitActor))
					{
						if (AEnemy* Enemy =Cast<AEnemy>(hitActor))
						{
							Enemy -> EnemyFSM -> OnMyTakeDamage(1);
							DamagedActorThisAttack.Add(hitActor);
						}
					}
				}
			}

			DrawDebugCapsule(GetWorld(),startLoc,80.f,15.f,Caprot,FColor::Green,false,0.2f);
			//}
		}
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
	if (GetCharacterMovement()->IsFalling())
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("OnActionAttackStart"))
	// 콤보를 할 수 있는 상태라면
	if (bIsAttack)
	{
		if (CurrentCombo >= MaxCombo - 1)
			return;
		
		// 콤보 공격 예약
		CurrentCombo++;
		ComboQueue.Add(CurrentCombo);
	}
	else
	{
		// 최초 공격 시작
		bIsAttack = true;
		CurrentCombo = 0;
		ComboQueue.Empty();
		PlayAnimMontage(PlayerDefaultAttackMontage, 0.8, "Attack0");
	}



	
	// if (bIsAttack == false)
	// {
	// 	bIsAttack = true;
	// 	CurrentCombo = 1;
	// 	
	// 	UAnimInstance* animInst = GetMesh()->GetAnimInstance();
	// 	if (animInst && PlayerDefaultAttackMontage)
	// 	{
	// 		animInst->Montage_Play(PlayerDefaultAttackMontage);
	// 		animInst->Montage_JumpToSection(FName("Attack0"));
	// 		bCanNextCombo = true;
	// 	}
	//
	// 	FHitResult OutHit;
	// 	FVector start = GetActorLocation();
	// 	FVector end = start + GetActorForwardVector() * 200.f;
	// 	FCollisionQueryParams Params;
	// 	Params.AddIgnoredActor(this);
	//
	// 	bool bhit = GetWorld()-> LineTraceSingleByChannel(OutHit,start,end,ECC_Visibility,Params);
	//
	// 	// 라인트레이스 디버그라인(빨간줄)
	// 	DrawDebugLine(GetWorld(), start,end,FColor::Red, false, 2.0f, 0, 1.0f);
	// 	
	// 	if (bhit==true)
	// 	{
	// 		if (AEnemy* Enemy =Cast<AEnemy>(OutHit.GetActor()))
	// 		{
	// 			Enemy -> EnemyFSM -> OnMyTakeDamage(1);
	// 		}
	// 	}
	// }
	// else if (bCanNextCombo)
	// {
	// 	bNextComboQueued = true;
	// 	bCanNextCombo = false;
	// 	GetCharacterMovement()-> MaxWalkSpeed = 0.f;
	// }
}

//공격 끝
void ADungeonAndKnightPlayer::OnActionAttackEnd(const FInputActionValue& value)
{
	
}

void ADungeonAndKnightPlayer::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify Begin: %s"), *NotifyName.ToString());
	
	if (NotifyName == "AttackStart")
	{
		UE_LOG(LogTemp, Warning,TEXT("AttackStart triggered"));
		DamagedActorThisAttack.Empty();
	}
	if (NotifyName == "ComboCheck")
	{
		// 만약 콤보 중인데
		if (bIsAttack)
		{
			// 큐가 비어있으면 콤보 종료
			if (ComboQueue.Num() == 0)
			{
				bIsAttack = false;
				CurrentCombo = 0;
			}
			else
			{
				//UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
				int32 attackType  = ComboQueue[0];
				FName NextSection = FName(*FString::Printf(TEXT("Attack%d"), attackType));
				PlayAnimMontage(PlayerDefaultAttackMontage, 1, NextSection);

				ComboQueue.RemoveAt(0);
			}
		}
		else
		{
			CurrentCombo = 0;
		}
	}
}




