// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "DungeonAndKnightPlayer.generated.h"

UCLASS()
class DUNGEONANDKNIGHT_API ADungeonAndKnightPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonAndKnightPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Direction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* SwordComp;
	
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerJump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerAttack;
	

	
	// 이동과 점프
	void OnActionMove(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	
	// 공격 시작과 끝
	void OnActionAttackStart(const FInputActionValue& value);
	void OnActionAttackEnd(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* PlayerDefaultAttackMontage;
	
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
	// void PlayAttackMontageSection(int32 ComboIndex);
	// void EnableComboInput();
	//void PlayNextComboSection();
	
	// 기본공격상태 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanNextCombo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNextComboQueued;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ComboAttackIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCombo;


	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int32 MaxCombo;
	
	
	
};
