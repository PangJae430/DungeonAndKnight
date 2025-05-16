// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include <tiffio.h>

#include "DungeonAndKnightPlayer.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);


}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ADungeonAndKnightPlayer* player = Cast<ADungeonAndKnightPlayer>(OtherActor);
	if (player)
	{
		UGameplayStatics::SetGamePaused(GetWorld(),true);
	}
}

void APortal::ClearRoom()
{
	Enemy-> EnemyFSM->KillCount;
}





