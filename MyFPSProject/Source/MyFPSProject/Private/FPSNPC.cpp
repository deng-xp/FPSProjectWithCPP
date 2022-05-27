// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSNPC.h"
#include "FPSGun.h"
#include "Components/BoxComponent.h"
#include "FPSProjectile.h"
#include "MyFPSProjectCharacter.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AFPSNPC::AFPSNPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//���ø���
	bReplicates=true;

	//������ײ
	FPSNPCBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FPSNPCBox"));
	FPSNPCBox->SetBoxExtent(FVector(60.0f));
	//���ú��ӵ�һ������ײ
	FPSNPCBox->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = FPSNPCBox;

	//���þ�̬������
	FPSNPCMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//FPSNPCMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//FPSNPCMeshComponent->SetSimulatePhysics(true);
	FPSNPCMeshComponent->SetupAttachment(RootComponent);

	//���ò���
	static ConstructorHelpers::FObjectFinder<UMaterial> CurMaterial(TEXT("'/Game/Materials/Projectile_Red.Projectile_Red'"));
	if (CurMaterial.Succeeded())
	{
		FPSNPCMaterial = UMaterialInstanceDynamic::Create(CurMaterial.Object, FPSNPCMeshComponent);
	}
	
	//�ٶȳ�ʼ��
	Speed = 200;
	//�����д�����ʼ��
	NumberOfHit=3;
}

// Called when the game starts or when spawned
void AFPSNPC::BeginPlay()
{
	Super::BeginPlay();
	FPSNPCBox->OnComponentHit.AddDynamic(this, &AFPSNPC::OnHit);
}

// Called every frame
void AFPSNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APlayerController* CurControl=UGameplayStatics::GetPlayerController(GetWorld(),0);

		//FVector FinalLocation = CurPawn->GetActorLocation();
		FVector FinalLocation = FVector(-1026,0,290);
		FVector NewLocation = this->GetActorLocation();
		FRotator NewRotation = (FinalLocation - NewLocation).Rotation();
		NewRotation.Pitch=0.0f;
		FVector2D MoveDirection = FVector2D((FinalLocation.X - NewLocation.X), (FinalLocation.Y - NewLocation.Y));

		//����̨һ����Χ�ں�����
		if (MoveDirection.Size() < 5)
		{
			Destroy();
		}

		MoveDirection = MoveDirection / MoveDirection.Size();
		MoveDirection = Speed * DeltaTime * MoveDirection;
		FVector DeltaLocation = FVector(MoveDirection.X, MoveDirection.Y, 0);

		NewLocation += DeltaLocation;
		//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), NewLocation.X, NewLocation.Y, NewLocation.Z);
		SetActorLocationAndRotation(NewLocation,NewRotation);

}

void AFPSNPC::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp,Warning,TEXT("ON HIT"));
	if (OtherActor != this)
	{
		AFPSProjectile* HitProjectile=Cast<AFPSProjectile,AActor>(OtherActor);
		AMyFPSProjectCharacter* HitCharacter= Cast<AMyFPSProjectCharacter, AActor>(OtherActor);
		if (HitProjectile != nullptr)
		{
			//this->FPSNPCMeshComponent->SetMaterial(0, FPSNPCMaterial);
			NumberOfHit--;
			if (NumberOfHit == 0)
			{
				Destroy();
			}
		}
		else if (HitCharacter != nullptr)
		{
			HitCharacter->SetHealth(-0.1);
			Destroy();
		}
	}
}