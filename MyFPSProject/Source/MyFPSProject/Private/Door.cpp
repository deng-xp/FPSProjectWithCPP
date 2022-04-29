// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��ײ�����ʼ��
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//������ײ
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//����������ײ
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	//�������Pawn������ײ��Ӧ���ص��¼���
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//������ײ��Χ
	TriggerBox->SetBoxExtent(FVector(50.0f, 50.0f, 20.0f));
	RootComponent = TriggerBox;

	//�ų�ʼ��
	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoorMesh->SetupAttachment(RootComponent);
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoorMesh->SetupAttachment(RootComponent);

	//������ʼ��
	OpenTime = 5.0f;
	IsDoorOpen = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//����������ʼ��
	DoorOpenWidth = fabs(RightDoorMesh->GetRelativeLocation().Y - LeftDoorMesh->GetRelativeLocation().Y);
	Speed = (DoorOpenWidth / 2) / OpenTime;
	UE_LOG(LogTemp, Warning, TEXT("The DoorOpenWidth is: %f"), DoorOpenWidth);
	UE_LOG(LogTemp, Warning, TEXT("The speed is: %f"), Speed);
	LeftDoorStartLocationY = LeftDoorMesh->GetRelativeLocation().Y;
	RightDoorStartLocationY = RightDoorMesh->GetRelativeLocation().Y;
	LeftDoorEndLocationY = LeftDoorStartLocationY - DoorOpenWidth / 2;
	RightDoorEndLocationY = RightDoorStartLocationY + DoorOpenWidth / 2;

	//���ص� 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);
	//�ر�ÿ֡����Tick()
	ADoor::SetActorTickEnabled(false);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDoorOpen)
	{
		ADoor::DoorOpen(DeltaTime);
	}
	else
	{
		ADoor::DoorClose(DeltaTime);
	}
}


//�Ŵ�
void ADoor::DoorOpen(float DeltaTime)
{
	FVector NewLocationLeft = LeftDoorMesh->GetRelativeLocation();
	FVector NewLocationRight = RightDoorMesh->GetRelativeLocation();
	if (NewLocationLeft.Y > LeftDoorEndLocationY && NewLocationRight.Y < RightDoorEndLocationY)
	{
		NewLocationLeft.Y -= DeltaTime * Speed;
		NewLocationRight.Y += DeltaTime * Speed;
		LeftDoorMesh->SetRelativeLocation(NewLocationLeft);
		RightDoorMesh->SetRelativeLocation(NewLocationRight);
	}
	else
	{
		//ֹͣÿ֡����
		ADoor::SetActorTickEnabled(false);
	}
}
//�����½�
void ADoor::DoorClose(float DeltaTime)
{
	FVector NewLocationLeft = LeftDoorMesh->GetRelativeLocation();
	FVector NewLocationRight = RightDoorMesh->GetRelativeLocation();
	if (NewLocationLeft.Y < LeftDoorStartLocationY && NewLocationRight.Y > RightDoorStartLocationY)
	{
		NewLocationLeft.Y += DeltaTime * Speed;
		NewLocationRight.Y -= DeltaTime * Speed;
		LeftDoorMesh->SetRelativeLocation(NewLocationLeft);
		RightDoorMesh->SetRelativeLocation(NewLocationRight);
	}
	else
	{
		//ֹͣÿ֡����
		ADoor::SetActorTickEnabled(false);
	}
}
//�ص��¼�
void  ADoor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Begin overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//��־λΪ����
		IsDoorOpen = true;
		//����ÿ֡����
		ADoor::SetActorTickEnabled(true);
	}
}
//�ص�����
void ADoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("End overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//��־λΪ����
		IsDoorOpen = false;
		//����ÿ֡����
		ADoor::SetActorTickEnabled(true);
	}
}