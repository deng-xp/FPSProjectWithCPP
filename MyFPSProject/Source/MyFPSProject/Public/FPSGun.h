// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPSGun.generated.h"

class UCameraComponent;
class UBoxComponent;
class USpringArmComponent;
class AMyFPSProjectCharacter;
class UTextRenderComponent;
UCLASS()
class MYFPSPROJECT_API AFPSGun : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFPSGun();
	//属性复制
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//枪(炮台)的组件
	//静态网格体组件，用于显示枪的外观
	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
	UStaticMeshComponent* GunMesh;
	//摄像机
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* GunCameraComponent;
	//碰撞组件
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* GunBoxComponent;
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CamerSpringArm;
	//发射物类型（只显示AFPSProjectile的子类与AFPSProjectile类）
	UPROPERTY(EditDefaultsOnly,Category=Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;
	//声音组件（开炮时的炮声）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSound;
	//文本组件，显示弹药不足时的提示信息
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* AmmoText;

	//输入变量
	//记录视角旋转角度
	FVector2D CameraInput;
	//开镜时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Zoomtime;
	//标记是否开镜
	bool IsZoom;
	//发射物射出位置相对相机的偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MuzzleOffset;
	//标记是否读取输入
	bool Input;
	//子弹数量(需同步)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int Ammo;
	//最大子弹数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmmo;
	//定时器句柄
	FTimerHandle TextTimerHandle;


protected:
	//记录与FPSGun发生碰撞的角色，用于将角色切换回去
	UPROPERTY()
	AMyFPSProjectCharacter* OverLapFPSCharacter;
	//记录当前控制器
	UPROPERTY()
	class AFPSPlayerController* CurControllerOfGun;

protected:
	//发射物发射函数，与Fire输入绑定
	UFUNCTION(Server, Reliable)
	void Fire();

	UFUNCTION()
	void StartFire();
	//摄像机上下转动
	void PitchCamera(float AxisValue);
	//摄像机左右转动
	void YawCamera(float AxisValue);
	//开镜
	void ZoomIn();
	//结束开镜
	void ZoomOut();
	//碰撞事件
	UFUNCTION()
	void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//按下“Interaction”事件（控制炮台,表示"交互"事件）
	UFUNCTION()
	void AcquireController();
	//显示准星UI的函数，在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayUI();
	//关闭UI
	UFUNCTION(BlueprintImplementableEvent)
	void CloseUI();
	//文本内容置为空
	void ClearText();

};
