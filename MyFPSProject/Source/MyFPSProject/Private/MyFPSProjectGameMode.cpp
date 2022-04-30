// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFPSProjectGameMode.h"
#include "MyFPSProjectCharacter.h"
#include "FPSGun.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMyFPSProjectGameMode::AMyFPSProjectGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	// set default pawn class to our Blueprinted character  
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_FPSGun"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	IsMissionAccomplished=true;
}

void AMyFPSProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMyFPSProjectGameMode::GameOver(APawn* MyPawn)
{
	UE_LOG(LogTemp,Warning,TEXT("Game over"));
	if (MyPawn)
	{
		//关闭输入
		MyPawn->DisableInput(nullptr);

		if (ViewActorClass)
		{
			/*切换视角*/
			//寻找世界中所有的可选类实例
			TArray<AActor*> ViewActor;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ViewActorClass, ViewActor);
			AActor* AimViewActor = nullptr;
			if (ViewActor.Num() != 0)
			{
				AimViewActor = ViewActor[0];
				APlayerController* MyController = Cast<APlayerController>(MyPawn->GetController());
				if (MyController)
				{
					MyController->SetViewTargetWithBlend(AimViewActor, 1.0, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("GameMode's ViewActorClass is nullptr!"));
		}
		MissionAcomplished();
	}
	
}

