// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBuff.generated.h"

class USphereComponent;
class UDecalComponent;
class ABuff;

UCLASS()
class MYFPSPROJECT_API ASpawnBuff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBuff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//��ײ��
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	//�������
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;
	//��������
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<ABuff> BuffClass;
	//��һ�����ٺ����������µ�
	UPROPERTY(EditInstanceOnly)
	float SpawnDuration;

	//��ʱ�����
	FTimerHandle SpawnTimerHandle;
	//���ڼ�¼��ǰbuff�Ƿ�Ϊ�գ����Ƿ�ʰȡ
	ABuff* BuffInstance;

	//����Buff
	void StartSpawnBuff();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//�����ص�ʱ���õĺ���
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
