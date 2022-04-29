// Fill out your copyright notice in the Description page of Project Settings.

//为角色增加buff(不实现具体的Buff，作为各种Buff的基类)
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
	//碰撞组件
	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* BuffBox;
	//显示外观的静态网格体组件
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BuffMesh;
	//Buff旋转速度
	UPROPERTY(EditAnywhere)
	float RotatorSpeed;

	//碰撞的函数(可在蓝图重写，进而实现不同buff效果）
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginOverlapBlueprint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
