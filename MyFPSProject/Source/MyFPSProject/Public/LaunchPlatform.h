// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPlatform.generated.h"

UCLASS()
class MYFPSPROJECT_API ALaunchPlatform : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Component")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UBoxComponent* BoxComponent;

	//µØ…‰ÀŸ∂»
	UPROPERTY(EditInstanceOnly,Category = "parameter")
	float LaunchSpeed;

	//µØ…‰Ω«∂»
	UPROPERTY(EditInstanceOnly, Category = "parameter")
	float LaunchAngle;

public:	
	// Sets default values for this actor's properties
	ALaunchPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
