// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPlatform.h"
#include "Components/BoxComponent.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
ALaunchPlatform::ALaunchPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	LaunchSpeed = 1500;
	LaunchAngle = 45;
}

// Called when the game starts or when spawned
void ALaunchPlatform::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPlatform::OnOverlapBox);
}

void ALaunchPlatform::OnOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator CurRotation = GetActorRotation();
	CurRotation.Pitch+=LaunchAngle;
	FVector LaunchVelocity=CurRotation.Vector()*LaunchSpeed;

	AMyFPSProjectCharacter* CurCharacter=Cast<AMyFPSProjectCharacter>(OtherActor);
	if (CurCharacter)
	{
		CurCharacter->LaunchCharacter(LaunchVelocity,true,true);
	}
}

