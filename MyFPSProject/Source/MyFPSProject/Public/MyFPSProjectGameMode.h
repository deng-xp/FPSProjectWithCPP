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

public:
	AMyFPSProjectGameMode();

	virtual void BeginPlay() override;

	void GameOver();
};



