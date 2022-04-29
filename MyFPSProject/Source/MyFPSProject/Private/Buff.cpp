// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "Components/BoxComponent.h"

// Sets default values
ABuff::ABuff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����ʼ��
	BuffBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	BuffBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BuffBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BuffBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BuffBox->SetBoxExtent(FVector(40.0f));
	RootComponent = BuffBox;

	BuffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuffMesh"));
	BuffMesh->SetupAttachment(RootComponent);

	RotatorSpeed = 40.0f;
}

// Called when the game starts or when spawned
void ABuff::BeginPlay()
{
	Super::BeginPlay();
	BuffBox->OnComponentBeginOverlap.AddDynamic(this, &ABuff::OnBeginOverlapBlueprint);
}

// Called every frame
void ABuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Buff��Ӧ�������岻����ת
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotatorSpeed * DeltaTime;
	SetActorRotation(NewRotation);
}
