// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"

class UBoxComponent;
class UDecalComponent;
UCLASS()
class MYFPSPROJECT_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExtractionZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//碰撞组件
	UPROPERTY(VisibleAnywhere,Category="Components")
	UBoxComponent* ExtractionZoneBoxComponent;
	//贴花组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* ExtractionZoneDecalComponent;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; 

};
