// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestBox.generated.h"

UCLASS()
class MYFPSPROJECT_API ATestBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestBox();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UBoxComponent* CollisionBoxComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UTextRenderComponent* TextComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
