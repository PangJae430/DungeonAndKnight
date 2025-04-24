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

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Direction;

	
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerJump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_PlayerAttack;

	
	
	void OnActionMove(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	void OnActionAttack(const FInputActionValue& value);

};
