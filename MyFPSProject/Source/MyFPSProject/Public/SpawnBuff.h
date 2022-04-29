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
	//碰撞球
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	//贴花组件
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;
	//生成类型
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<ABuff> BuffClass;
	//上一个销毁后隔多久生成新的
	UPROPERTY(EditInstanceOnly)
	float SpawnDuration;

	//定时器句柄
	FTimerHandle SpawnTimerHandle;
	//用于记录当前buff是否为空，即是否被拾取
	ABuff* BuffInstance;

	//生成Buff
	void StartSpawnBuff();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//发生重叠时调用的函数
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
