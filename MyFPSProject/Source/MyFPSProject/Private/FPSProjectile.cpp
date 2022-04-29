// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//��ʼ����ײ���
	CollisionComponent=CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
	//��ʼ���뾶
	CollisionComponent->InitSphereRadius(10.0f);
	//����Ŀ��Ԥ�����ײ�ļ���ʼ����ײ
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	//����ײ�������Ϊ�����
	RootComponent=CollisionComponent;
	//��ʼ����̬���������
	ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMeshComponent>Mesh(TEXT("'/Game/Geometry/Meshes/CubeMaterial.CubeMaterial'"));
	if (Mesh.Succeeded())
	{
		ProjectileMeshComponent=Mesh.Object;
	} 

	//��ʼ�����������
	//ֱ�ӷ��ʲ����ļ�������ֵ
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/Projectile_Red.Projectile_Red'"));
	if (Material.Succeeded())
	{
		//���ò��ʣ��������뷢���������������
		ProjectileMaterialInstance=UMaterialInstanceDynamic::Create(Material.Object,ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0,ProjectileMaterialInstance);
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//ͨ���������ƶ���������������ƶ�
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//��Ҫ���Ƶ����
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//���ó�ʼ�ٶ�
		ProjectileMovementComponent->InitialSpeed=6000.0f;
		//��������ٶ�
		ProjectileMovementComponent->MaxSpeed=6000.0f;
		//�ӵ�����ת����ÿһ֡�и��£���ƥ�����ٶ�
		ProjectileMovementComponent->bRotationFollowsVelocity=true;
		//���÷���
		ProjectileMovementComponent->bShouldBounce=false;
		//���÷���ϵ����������������ǿ��
		ProjectileMovementComponent->Bounciness=0.3f;
		//��������ϵ��
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//�����������ڣ�����Զ����٣�
		InitialLifeSpan=2.0f;	//2�������
	}

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	//����ײ�¼�
	CollisionComponent->OnComponentHit.AddDynamic(this,&AFPSProjectile::OnHit);
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�ٶ�Ϊ0ʱ���٣�Ϊ0��ʾ�Ѿ�������ײ�ˣ�
	if (ProjectileMovementComponent->Velocity == FVector(0.0f))
	{
		this->Destroy();
	}
}

//�������ٶȣ�ʸ�����������䷽�򣻲���ShootDirection�ɿ������õ����룩
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}

//��ײ�¼�;������Hit�洢��ײ����λ�õ���Ϣ
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//��֤���Ƿ������뷢������ײ���ұ����е�����п�������ģ��ʱ��ʩ�ӳ��
	if (OtherActor != this&& OtherComp->IsSimulatingPhysics())
	{
		//��ָ��λ�ã�Hit.ImpactPoint��ʩ���ص��С�ĳ����ProjectileMovementComponent->Velocity*100.0f��
		OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		//��ʾ����Ч��
		if (FireParticle != nullptr)
		{
			UGameplayStatics::SpawnEmitterAttached(FireParticle, OtherComp, "Fired");
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),FireParticle,OtherActor->GetActorTransform());
		}
	}
}
