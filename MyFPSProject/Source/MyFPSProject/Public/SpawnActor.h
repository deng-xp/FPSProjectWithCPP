// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnActor.generated.h"

UCLASS()
class MYFPSPROJECT_API ASpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//����Actor��ʱ����
	UPROPERTY(EditAnywhere)
	float DeltaTimeOfSpawnActor;
	//����Actor������
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFPSNPC> ActorClassBeSpawned;
	
	//��ʱ�����
	FTimerHandle SpawnActorTimeHandle;

	//����SpawnActorΪ���ĵľ��η�Χ������Actor���·���������ָ�����ɷ�Χ(�����һ��)
	UPROPERTY(EditAnywhere)
	float WidthX;
	UPROPERTY(EditAnywhere)
	float LengthY;

protected:
	//����
	//����Actor�ĺ���
	void SpawnNPC();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
