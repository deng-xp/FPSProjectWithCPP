// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameStateBase.h"
#include "FPSPlayerController.h"

void AFPSGameStateBase::DisableInputofThePawn_Implementation(APawn* CurPawn,bool IsAcomplished)
{
	//��ʾUI���桢��������
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* CurController=Cast<AFPSPlayerController>(It->Get());
		if (CurController&&CurController->IsLocalController())
		{
			CurController->OnMissionAcomplished(IsAcomplished);
			APawn* Pawn = CurController->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(nullptr);
			}
		}
	}
}
