// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
class AFPSGun;
UCLASS()
class MYFPSPROJECT_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	AFPSPlayerController();
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY()
	class AFPSGun* HitGun;

	//显示游戏结束的UI界面
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionAcomplished(bool IsAcomplished);

	UFUNCTION(Server,Reliable)
	void ChangePawn(APawn* HitGuna);
	void ChangePawn_Implementation(APawn* HitGuna);
};
