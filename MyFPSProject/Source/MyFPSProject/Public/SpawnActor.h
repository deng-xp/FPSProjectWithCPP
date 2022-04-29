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
	//生成Actor的时间间隔
	UPROPERTY(EditAnywhere)
	float DeltaTimeOfSpawnActor;
	//生成Actor的类型
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFPSNPC> ActorClassBeSpawned;
	
	//计时器句柄
	FTimerHandle SpawnActorTimeHandle;

	//在以SpawnActor为中心的矩形范围内生成Actor，下方参数用于指定生成范围(长宽的一半)
	UPROPERTY(EditAnywhere)
	float WidthX;
	UPROPERTY(EditAnywhere)
	float LengthY;

protected:
	//函数
	//生成Actor的函数
	void SpawnNPC();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
