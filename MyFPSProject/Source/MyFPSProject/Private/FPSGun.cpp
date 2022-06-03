// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGun.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSProjectile.h"
#include "MyFPSProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerController.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AFPSGun::AFPSGun()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���ø���
	bReplicates = true;

	//������ʼ��
	Zoomtime = 0.3f;
	IsZoom = false;
	Input=false;
	//�ӵ�����
	Ammo = 30;
	MaxAmmo = 30;

	//�����ʼ��
	//�������
	GunBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GunBoxComponent"));
	GunBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GunBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	GunBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	GunBoxComponent->SetBoxExtent(FVector(60.0f, 20.0f, 20.0f));
	if (RootComponent == nullptr)
	{
		RootComponent = GunBoxComponent;
	}
	//���ɱ����
	CamerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerSpringArm"));
	CamerSpringArm->SetupAttachment(RootComponent);
	//���õ��ɱ۳���
	CamerSpringArm->TargetArmLength = 30.0f;
	//����������ӳ٣���Ϊ�棬��������ͺ�������ƽ���˶���
	CamerSpringArm->bEnableCameraLag = true;
	//����������ӳ��ٶ�
	CamerSpringArm->CameraLagSpeed = 0.1f;
	//���õ��ɱ����λ����Ƕ�
	CamerSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	//������
	GunCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("GunCameraComponent"));
	//ȷ����������Ϊ�գ�Ϊ���򲻽��к�������
	check(GunCameraComponent != nullptr);
	//��������ӵ����ɱ����
	GunCameraComponent->SetupAttachment(CamerSpringArm, USpringArmComponent::SocketName);

	//�������������
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	check(GunMesh != nullptr);
	GunMesh->SetupAttachment(RootComponent);

	//�ı����
	AmmoText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("AmmoText"));
	AmmoText->SetHorizontalAlignment(EHTA_Center);
	AmmoText->SetWorldSize(50.0f);
	AmmoText->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFPSGun::BeginPlay()
{
	Super::BeginPlay();
	//���ص���ʼ�¼�����������
	GunBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSGun::OnBeginOverLap);
	//���ص������¼����ر�����
	GunBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFPSGun::OnEndOverLap);
}

// Called every frame
void AFPSGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***************�ӽǵ������**************/
	//��תActor�������������Actor�໥�󶨣����Actorת����CameraҲ�ᶯ
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += CameraInput.Y;
	//������ת�Ƕ���һ����Χ��
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.0f, 80.f);
	SetActorRotation(NewRotation);
	NewRotation.Yaw += CameraInput.X;
	NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, -75.0f, 75.0f);
	SetActorRotation(NewRotation);

	/*****************�������*****************/
	//��ȡ�������Ұ�Ƕȣ������Ƕ�Ϊ30�㣬����Ϊ90�㣩
	float CurView = GunCameraComponent->FieldOfView;
	if (IsZoom && CurView > 30.0f)
	{
		CurView -= (60.0 / Zoomtime) * DeltaTime;
		CurView=FMath::Clamp(CurView,30.0f,90.0f);
		GunCameraComponent->SetFieldOfView(CurView);
	}
	else if (!IsZoom && CurView < 90.0f)
	{
		CurView += (60.0 / Zoomtime) * DeltaTime;
		CurView = FMath::Clamp(CurView, 30.0f, 90.0f);
		GunCameraComponent->SetFieldOfView(CurView);
	}
}

// Called to bind functionality to input
void AFPSGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//��Fire����
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSGun::StartFire);
	//��ͷ��ת
	PlayerInputComponent->BindAxis("Turn", this, &AFPSGun::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSGun::PitchCamera);
	//���ؾ�
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPSGun::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPSGun::ZoomOut);
}
//�����﷢��
void AFPSGun::Fire_Implementation()
{
	if (Ammo <= 0)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Please replenish your ammunition"),false,FVector2D(1.5,1.5));
		AmmoText->SetText(TEXT("Please replenish your ammunition"));
		GetWorldTimerManager().SetTimer(TextTimerHandle,this,&AFPSGun::ClearText,2.0f,true);
		return;
	}
	else if (ProjectileClass)
	{
		//��ȡ����ı任
		FVector CameraLocation=GunCameraComponent->GetComponentLocation();
		FRotator CameraRotation=GunCameraComponent->GetComponentRotation();
		//GetActorEyesViewPoint(CameraLocation,CameraRotation);
		MuzzleOffset.Set(62.0f,0.0f,0.0f);
		//���÷���������λ�ã�ƫ��λ��������������Ƶģ��������������ռ��ڣ�Ҫ����ת����������ϵ��
		FVector MuzzleLocation=CameraLocation+FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation=CameraRotation;
		//���ɷ�����
		UWorld* World=GetWorld();
		if (World)
		{
			//���ò���
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner=this;
			SpawnParams.Instigator=GetInstigator();

			//���ɷ�����(���͡�λ�ã�
			AFPSProjectile* Projectile=World->SpawnActor<AFPSProjectile>(ProjectileClass,MuzzleLocation,MuzzleRotation,SpawnParams);
			if (Projectile)
			{
				//���÷�����Ĺ켣���õ������﷢��ķ��򣬴�����AFPSProjectile���У��������ó�ʼ�ٶȣ�
				FVector LauchDirection=MuzzleRotation.Vector();
				Projectile->FireInDirection(LauchDirection);
				//�ӵ�������1
				Ammo--;
				UE_LOG(LogTemp, Log, TEXT("Ammo is: %i"),Ammo);
			}
		}
		//�������
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this,FireSound,GetActorLocation());
		}
	}
}
void AFPSGun::StartFire()
{
	Fire_Implementation();
}
//���������ת��
void AFPSGun::PitchCamera(float AxisValue)
{
	CameraInput.Y = -AxisValue;
}
//���������ת��
void AFPSGun::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
//����
void AFPSGun::ZoomIn()
{
	//UE_LOG(LogTemp, Warning, TEXT("Zoom"));
	IsZoom=true;
}
//��������
void AFPSGun::ZoomOut()
{
	//UE_LOG(LogTemp, Warning, TEXT("ZoomOut"));
	IsZoom=false;
}
//��ʼ�ص�
void AFPSGun::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Begin Overlap"));
	if (OtherActor != this)
	{
		AMyFPSProjectCharacter* OverLapCharacter = Cast<AMyFPSProjectCharacter>(OtherActor);
		AFPSPlayerController* CurCharacterController=nullptr;
		if (OverLapCharacter != nullptr)
		{
			CurCharacterController = Cast<AFPSPlayerController>(OverLapCharacter->GetController());
			CurControllerOfGun = (CurCharacterController);
			if (OverLapCharacter->GetLocalRole()==ROLE_Authority&&CurCharacterController)
			{
				FString curname = CurCharacterController->GetName();
				UE_LOG(LogTemp,Warning,TEXT("The current controller's name is %s"),*curname);

				//�˴���ӳ�䣺����������ģʽ�£��пͻ��˳䵱������
				if (CurCharacterController->InputComponent != nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("The current controller's inputcomponent is not null,Server"));
					CurCharacterController->InputComponent->BindAction("Interaction", IE_Pressed, this, &AFPSGun::AcquireController);
				}
			}
			if (OverLapCharacter->GetLocalRole() == ROLE_AutonomousProxy)
			{
				//�ڿͻ��˰�����ӳ��
				if (CurCharacterController->InputComponent != nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("The current controller's inputcomponent is not null,client"));
					CurCharacterController->InputComponent->BindAction("Interaction", IE_Pressed, this, &AFPSGun::AcquireController);
				}
			}
			//��¼�����ص������Ľ�ɫ
			OverLapFPSCharacter = OverLapCharacter;
			//���ݽ�ɫ�Ƿ���е�ҩ�����Ƿ������̨��ҩ��
			Ammo = (OverLapCharacter->HasAmmo) ? MaxAmmo : Ammo;
		}
	}
}
//�ص�����
void AFPSGun::OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Gun end Overlap"));
	//����� ����Pawn�л�������
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->RemoveActionBinding("Interaction",IE_Pressed);
	if (OverLapFPSCharacter->GetLocalRole() == ROLE_AutonomousProxy||(OverLapFPSCharacter->GetLocalRole() == ROLE_AutonomousProxy && CurControllerOfGun->
		InputComponent != nullptr))
	{
		CurControllerOfGun->InputComponent->RemoveActionBinding("Interaction", IE_Pressed);
	}
}

void AFPSGun::AcquireController()
{
	UE_LOG(LogTemp,Warning,TEXT("F bind"));
	AFPSPlayerController* MyController = CurControllerOfGun;
	//�ڷ�����ִ�У��ͻ��˵���
	if (MyController->GetLocalRole()==ROLE_AutonomousProxy|| MyController->GetLocalRole() == ROLE_Authority)
	{
		if (MyController->GetPawn() != this)
		{
			DisplayUI();
			//ͨ���������е�RPC����ʵ�ֽ�ɫ�л�
			MyController->ChangePawn(this);
			FString nameofcontroller = MyController->GetName();
			UE_LOG(LogTemp, Error, TEXT("The current controller's inputcomponent is %s"), *nameofcontroller);
		}
		else
		{
			CloseUI();
			MyController->ChangePawn(OverLapFPSCharacter);
			FString nameofcontroller = MyController->GetName();
			UE_LOG(LogTemp, Error, TEXT("The current controller's inputcomponent is %s"), *nameofcontroller);
		}
	}
}

void AFPSGun::ClearText()
{
	//�ı���Ϣ����
	AmmoText->SetText(TEXT(""));
}

void AFPSGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Ҫ���Ƶ�����
	DOREPLIFETIME(AFPSGun,Ammo);
}