// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyFPSProjectGameMode.generated.h"

class AMyFPSProjectCharacter;
class AFPSGun;
UCLASS(minimalapi)
class AMyFPSProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//游戏结束后切换视角位置的类
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ViewActorClass;
	

public:
	AMyFPSProjectGameMode();

	virtual void BeginPlay() override;

	void GameOver(APawn* MyPawn);

	//任务完成(此处用来显示游戏结束的UI界面)
	UFUNCTION(BlueprintImplementableEvent)
	void MissionAcomplished();

	//用于表示输出那个UI界面
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMissionAccomplished;
};



