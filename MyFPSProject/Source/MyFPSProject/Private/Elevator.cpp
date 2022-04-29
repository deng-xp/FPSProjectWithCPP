// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
AElevator::AElevator()
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

	//����ƽ̨��ʼ��
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	//ʱ�����ʼ��
	ElevatorTimelineComp=CreateDefaultSubobject<UTimelineComponent>(TEXT("ElevatorTimelineComp"));
	//�ƹ��ʼ��
	ElevetorLight=CreateDefaultSubobject<ULightComponent>(TEXT("ElevetorLight"));
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	//���������󶨵�UpdataTimelineComp��������Timeline�������ʱ�����õĺ���
	UpdateFunctionFloat.BindDynamic(this,&AElevator::UpdataTimelineComp);
	//��������������º�����
	if (ElevatorTimelineFloatCurve)
	{
		ElevatorTimelineComp->AddInterpFloat(ElevatorTimelineFloatCurve,UpdateFunctionFloat);
	}

	//����ײ�¼�
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnBoxOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnBoxEndOverlap);
	//���õƲ��ɼ�
	if (ElevetorLight)
	{
		ElevetorLight->SetVisibility(false);
	}
	//��¼��ʼλ�ã������������������½�
	StartLocationZ=GetActorLocation().Z;
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//OutputΪTimeline�ĺ���ֵ���
void AElevator::UpdataTimelineComp(float Output)
{
	FVector NewLocation=GetActorLocation();
	NewLocation.Z=Output;
	//��Timeline�����ֵΪ�µ�Z����
	AElevator::SetActorLocation(NewLocation);
}

//�ص��¼�
void  AElevator::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Begin overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		if (GetActorLocation().Z == StartLocationZ)
		{
			ElevatorTimelineComp->Play();	//����
		}
		else
		{
			ElevatorTimelineComp->Reverse();	//���򲥷ţ����½�
		}
		//����
		if (ElevetorLight)
		{
			ElevetorLight->SetVisibility(true);
		}	
	}
}
//�ص�����
void AElevator::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("End overlap"));
	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//�ص�
		if (ElevetorLight)
		{
			ElevetorLight->SetVisibility(false);
		}
	}
}