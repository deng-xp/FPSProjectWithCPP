// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBuff.h"
#include "Buff.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawnBuff::ASpawnBuff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(60.0f);
	RootComponent=SphereComponent;

	DecalComponent=CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));
	DecalComponent->DecalSize=FVector(50.0,60.0,60.0);
	DecalComponent->SetupAttachment(RootComponent);

	SpawnDuration=10.0f;
}

// Called when the game starts or when spawned
void ASpawnBuff::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnBuff();
}

// Called every frame
void ASpawnBuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnBuff::StartSpawnBuff()
{
	if (BuffClass == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Please choose the class of Buff you want to spawn!"));
		return;
	}
	UWorld* CurWorld=GetWorld();
	if (CurWorld)
	{
		FActorSpawnParameters SpawnParams;
		//在生成Actor时会碰撞如何处理
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
		BuffInstance = CurWorld->SpawnActor<ABuff>(BuffClass, GetTransform(), SpawnParams);
	}
}

void ASpawnBuff::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (BuffInstance)
	{
		BuffInstance=nullptr;
		GetWorldTimerManager().SetTimer(SpawnTimerHandle,this,&ASpawnBuff::StartSpawnBuff,SpawnDuration,true);
	}
}