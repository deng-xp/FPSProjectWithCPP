// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSNPC.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API AFPSNPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//��¼�����д���
	UPROPERTY(VisibleAnywhere)
	int NumberOfHit;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//��ʾNPC��۵����
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* FPSNPCMeshComponent;
	//��ײ��
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* FPSNPCBox;
	//����
	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* FPSNPCMaterial;
	//NPC�ƶ��ٶ�
	UPROPERTY(EditAnywhere)
		float Speed;

	//��ײ�����¼�
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
