// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFPSProjectGameMode.h"
#include "MyFPSProjectCharacter.h"
#include "FPSGun.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameStateBase.h"


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
	GameStateClass=AFPSGameStateBase::StaticClass();
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
		//关闭输入，并在各个客户端显示UI
		//MyPawn->DisableInput(nullptr);
		AFPSGameStateBase* GS=GetGameState<AFPSGameStateBase>();
		if (GS)
		{
			GS->DisableInputofThePawn(MyPawn,IsMissionAccomplished);
		}
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
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* MyController = It->Get();
					if (MyController)
					{
						MyController->SetViewTargetWithBlend(AimViewActor, 1.0, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("GameMode's ViewActorClass is nullptr!"));
		}
		//MissionAcomplished();
	}
}

