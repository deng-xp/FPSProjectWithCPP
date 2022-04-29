// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Elevator.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API AElevator : public AActor
{
	GENERATED_BODY()

	//��̬���������������ƽ̨��
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* PlatformMesh;
	//��ײ���
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;
	//�ƹ����
	UPROPERTY(EditAnywhere)
	class ULightComponent* ElevetorLight;

	//���ж��������ʱ�������
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* ElevatorTimelineComp;
	//�����ʲ�������Timeline�仯����
	UPROPERTY(EditAnywhere)
	UCurveFloat* ElevatorTimelineFloatCurve;
	//���ڴ�����¹���¼��ĸ�����ǩ��(ί�н�������ֵ����ʱ�ĺ�����ǩ����
	FOnTimelineFloat UpdateFunctionFloat;

	//��¼���ݳ�ʼλ��
	UPROPERTY(VisibleAnywhere)
	float StartLocationZ;

public:
	// Sets default values for this actor's properties
	AElevator();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//���µ���λ�õĺ���
	UFUNCTION()
	void UpdataTimelineComp(float Output);
	
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
