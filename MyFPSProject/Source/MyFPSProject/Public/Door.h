// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

	//ʹ�����Һ����ţ������������
	//��̬�������������ʾ����ţ�
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* LeftDoorMesh;
	//��̬�������������ʾ�ұ��ţ�
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* RightDoorMesh;
	//��ײ���
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	//�ŵ�ԭʼλ��
	UPROPERTY()
	float LeftDoorStartLocationY;
	UPROPERTY()
	float RightDoorStartLocationY;
	//�ŵļ���λ��
	UPROPERTY()
	float LeftDoorEndLocationY;
	UPROPERTY()
	float RightDoorEndLocationY;

	//�Ŵ򿪵Ĵ�С���������ŵĳ�ʼλ�����
	UPROPERTY()
	float DoorOpenWidth;
	//����ȫ�������ʱ��
	UPROPERTY(EditAnywhere, Category = "time")
	float OpenTime;
	//�Ŵ򿪵��ٶ�
	UPROPERTY()
	float Speed;
	//��־λ�����ڱ�־���Ǵ򿪻��ǹر�
	bool IsDoorOpen;

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�Ŵ�
	UFUNCTION()
	void DoorOpen(float DeltaTime);
	//�Źر�
	UFUNCTION()
	void DoorClose(float DeltaTime);
	//�ص���ʼ��Ӧ�¼�
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//�ص������¼�
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
