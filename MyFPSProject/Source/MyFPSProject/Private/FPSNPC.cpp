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
	
	//启用复制
	bReplicates=true;

	//设置碰撞
	FPSNPCBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FPSNPCBox"));
	FPSNPCBox->SetBoxExtent(FVector(60.0f));
	//采用和子弹一样的碰撞
	FPSNPCBox->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = FPSNPCBox;

	//设置静态网格体
	FPSNPCMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//FPSNPCMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//FPSNPCMeshComponent->SetSimulatePhysics(true);
	FPSNPCMeshComponent->SetupAttachment(RootComponent);

	//设置材质
	static ConstructorHelpers::FObjectFinder<UMaterial> CurMaterial(TEXT("'/Game/Materials/Projectile_Red.Projectile_Red'"));
	if (CurMaterial.Succeeded())
	{
		FPSNPCMaterial = UMaterialInstanceDynamic::Create(CurMaterial.Object, FPSNPCMeshComponent);
	}
	
	//速度初始化
	Speed = 200;
	//被击中次数初始化
	NumberOfHit=3;
}

void AFPSNPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//要复制的变量
	DOREPLIFETIME(AFPSNPC, NumberOfHit);
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

		//在炮台一定范围内后销毁
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