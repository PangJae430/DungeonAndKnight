// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class DUNGEONANDKNIGHT_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ADungeonAndKnightPlayer* Player;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AEnemy* Enemy;
	
	int32 CurrentKillCount=0;

	UFUNCTION()
	void ClearRoom();
};


