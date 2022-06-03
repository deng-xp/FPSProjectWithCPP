// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"
#include "FPSNPC.h"
#include "kismet/GameplayStatics.h"
#include "FPSGun.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpawnActor::ASpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeltaTimeOfSpawnActor = 2.0f;
	WidthX = 250.0f;
	LengthY = 250.0f;
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(SpawnActorTimeHandle, this, &ASpawnActor::SpawnNPC, DeltaTimeOfSpawnActor, true);
	}
}

void ASpawnActor::SpawnNPC()
{
	UWorld* CurWorld=GetWorld();
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	//�����������ʵ�֣�NPC���������һ����Χ��
	float randx = FMath::FRandRange(-1 * WidthX, WidthX);
	float randy = FMath::FRandRange(-1 * LengthY, LengthY);
	Location.X+=randx;
	Location.Y+=randy;

	//APawn* CurPawn = UGameplayStatics::GetPlayerPawn(CurWorld, 0);
	//AFPSGun* CurGun = Cast<AFPSGun, APawn>(CurPawn);

	//ֻ���ڿ��ƶ���Ϊ��̨ʱ����NPC
	//if (CurWorld && CurGun != nullptr)
	//{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		AFPSNPC* NPC = CurWorld->SpawnActor<AFPSNPC>(ActorClassBeSpawned, Location, Rotation, SpawnParams);
	//}
}
// Called every frame
void ASpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

