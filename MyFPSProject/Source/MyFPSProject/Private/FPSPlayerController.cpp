// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "FPSGun.h"

AFPSPlayerController::AFPSPlayerController(){

}
void AFPSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}
void AFPSPlayerController::ChangePawn_Implementation(APawn* HitGuna)
{
	OnPossess(HitGuna);
}