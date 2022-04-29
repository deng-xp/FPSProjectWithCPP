// Fill out your copyright notice in the Description page of Project Settings.

//Ϊ��ɫ����buff(��ʵ�־����Buff����Ϊ����Buff�Ļ���)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Buff.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API ABuff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//��ײ���
	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* BuffBox;
	//��ʾ��۵ľ�̬���������
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BuffMesh;
	//Buff��ת�ٶ�
	UPROPERTY(EditAnywhere)
	float RotatorSpeed;

	//��ײ�ĺ���(������ͼ��д������ʵ�ֲ�ͬbuffЧ����
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginOverlapBlueprint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
