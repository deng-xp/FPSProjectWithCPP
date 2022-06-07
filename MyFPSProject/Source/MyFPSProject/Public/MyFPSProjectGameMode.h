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
	//��Ϸ�������л��ӽ�λ�õ���
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ViewActorClass;
	

public:
	AMyFPSProjectGameMode();

	virtual void BeginPlay() override;

	void GameOver(APawn* MyPawn);

	//�������(�˴�������ʾ��Ϸ������UI����)
	//UFUNCTION(BlueprintImplementableEvent)
	//void MissionAcomplished();

	//���ڱ�ʾ����Ǹ�UI����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMissionAccomplished;
};



