// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "FPSProjectile.generated.h"

UCLASS()
class MYFPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/****************��Ա����*********************/
	//��������
	//������ײ��������ڼ���ӵ�����ײ
	UPROPERTY(VisibleDefaultsOnly, Category = Project)
	USphereComponent* CollisionComponent;
	//�������ƶ���������Ʒ����ȥ���ӵ��ƶ���
	//����������������������صĲ�������������ٶȡ���ʼ�ٶȵ�
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	//������������
	UPROPERTY(VisibleAnywhere,Category=Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;
	//��������ʣ���̬���ʣ������������޸ģ�
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;
	//���ӣ������������ʾ
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UParticleSystem* FireParticle;
	
	
	/****************��Ա����*********************/
	//��ʼ��������ķ��䷽��
	void FireInDirection(const FVector& ShootDirection);
	//��ײ����
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
