// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFPSProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "FPSGun.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyFPSProjectGameMode.h"

//////////////////////////////////////////////////////////////////////////
// AMyFPSProjectCharacter

AMyFPSProjectCharacter::AMyFPSProjectCharacter()
{
	//����ÿ֡����
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//������ʼ��
	Health = 1.0f;
	Energy = 1.0f;
	ConsumeEnergyOfJump = 0.1f;
	ConsumeEnergyOfRun = 0.1f;
	EnergyRegenerationRate = 0.005f;
	WalkSpeed = 600.0f;
	MaxSpeed = 1200.0f;
	Speed = WalkSpeed;
	HasAmmo=false;
	IsDied=false;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ���ý�ɫ�˶���Ϣ
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
void AMyFPSProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyFPSProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFPSProjectCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindKey(EKeys::K, IE_Pressed, this, &ACharacter::Jump);[�����󶨳ɹ�����Ҫ�ڡ�MyFPSProject.Bulid.cs�ļ��м�"slate"]

	//�󶨼�����
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyFPSProjectCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyFPSProjectCharacter::StopRun);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyFPSProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyFPSProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyFPSProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyFPSProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyFPSProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyFPSProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyFPSProjectCharacter::OnResetVR);
}


void AMyFPSProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Energy < 1.0)
	{
		//ÿ֡����energy
		Energy += EnergyRegenerationRate * DeltaTime;
		Energy = FMath::Clamp(Energy, 0.0f, 1.0f);
	}
}

void AMyFPSProjectCharacter::StartJump()
{
	//�����㹻ʱ������Ծ������Ծ�����
	if (Energy >= ConsumeEnergyOfJump)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 1.0f;
		Energy = Energy - ConsumeEnergyOfJump;
		Energy = FMath::Clamp(Energy, 0.0f, 1.0f);
	}
}

void AMyFPSProjectCharacter::Run()
{
	Speed = MaxSpeed;
}

void AMyFPSProjectCharacter::StopRun()
{
	Speed = WalkSpeed;
}

void AMyFPSProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		if (Speed == MaxSpeed)
		{
			float consume = ConsumeEnergyOfRun * GetWorld()->DeltaTimeSeconds;
			if (Energy >= consume)
			{
				Energy = Energy - consume;
				Energy = FMath::Clamp(Energy, 0.0f, 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("Current Energy is %f"), ConsumeEnergyOfRun);
			}
			else
			{
				Speed = WalkSpeed;
			}
		}
		//�޸��ٶ�
		GetCharacterMovement()->MaxWalkSpeed = Speed;

		

		AddMovementInput(Direction, Value);
	}
}

void AMyFPSProjectCharacter::OnHealthChange()
{
	if (Health == 0 && !IsDied)
	{
		IsDied=true;

		//����Gamemode����Ϸ��������
		AMyFPSProjectGameMode* CurGameMode=Cast<AMyFPSProjectGameMode,AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		CurGameMode->IsMissionAccomplished=false;
		CurGameMode->GameOver(this);
		
		InitialLifeSpan=5.0f;
	}
}

void AMyFPSProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Ҫ���Ƶı���
	DOREPLIFETIME(AMyFPSProjectCharacter, Health);
	DOREPLIFETIME(AMyFPSProjectCharacter, Energy);
}


void AMyFPSProjectCharacter::OnRep_Health()
{
	OnHealthChange();
}

void AMyFPSProjectCharacter::SetHealth(float Damage)
{
	Health+=Damage;
	Health = FMath::Clamp(Health, 0.0f, 1.0f);
	OnHealthChange();
}


void AMyFPSProjectCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyFPSProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyFPSProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyFPSProjectCharacter::OnResetVR()
{
	// If MyFPSProject is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MyFPSProject.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyFPSProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyFPSProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}




