// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "AFPSGunHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYFPSPROJECT_API AAFPSGunHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//HUD���Ƶ���Ҫ���ã�������д
	virtual void DrawHUD() override;

protected:

};
