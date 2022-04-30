// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "MyFPSProjectGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//组件初始化
	ExtractionZoneBoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("ExtractionZoneBoxComponent"));
	ExtractionZoneBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExtractionZoneBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExtractionZoneBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	ExtractionZoneBoxComponent->SetBoxExtent(FVector(100.0f));
	RootComponent=ExtractionZoneBoxComponent;

	ExtractionZoneDecalComponent=CreateDefaultSubobject<UDecalComponent>(TEXT("ExtractionZoneDecalComponent"));
	ExtractionZoneDecalComponent->DecalSize=FVector(100.0f);
	ExtractionZoneDecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSExtractionZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp,Log,TEXT("Extraction Zone begin overlap"));
	AMyFPSProjectGameMode* CurMode=Cast<AMyFPSProjectGameMode>(GetWorld()->GetAuthGameMode());
	AMyFPSProjectCharacter* MyCharacter=Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyCharacter)
	{
		CurMode->GameOver(MyCharacter);
		//CurMode->MissionAcomplished();
	}

}

