// Fill out your copyright notice in the Description page of Project Settings.

//��תͬʱ�����ƶ���ƽ̨
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class MYFPSPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

protected:
	//��ʾƽ̨��۵����
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlatformMesh;
	//�ƶ��ٶ�
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	//ת���ٶ�
	UPROPERTY(EditAnywhere)
	float RotateSpeed;
	//Sin�����ƶ�����λ�ʵ�ִ�λ�ƶ���
	UPROPERTY(EditAnywhere)
	float PhaseDifference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
