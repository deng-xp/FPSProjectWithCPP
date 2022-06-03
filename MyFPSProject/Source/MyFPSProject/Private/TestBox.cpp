// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBox.h"
#include "MyFPSProjectCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATestBox::ATestBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CreateDefaultSubobject"));
	CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	CollisionBoxComponent->SetBoxExtent(FVector(50.0f,50.0f,50.0f));

	TextComponent=CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	TextComponent->SetupAttachment(CollisionBoxComponent);
	TextComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATestBox::BeginPlay()
{
	Super::BeginPlay();
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATestBox::OnBeginOverlap);
}

// Called every frame
void ATestBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyFPSProjectCharacter* MyCharacter=Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyCharacter)
	{
		if (MyCharacter->GetLocalRole() == ROLE_Authority)
		{
			MyCharacter->OnBeginOverlapWithTestBox();
			if (MyCharacter->GetRemoteRole() == ROLE_SimulatedProxy)
			{
				TextComponent->SetVisibility(true);
			}
		}
		if (MyCharacter->GetLocalRole() == ROLE_AutonomousProxy)
		{
			TextComponent->SetVisibility(true);
		}
	}
}