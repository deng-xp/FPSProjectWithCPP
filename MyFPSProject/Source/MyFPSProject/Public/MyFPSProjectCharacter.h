// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "MyFPSProjectCharacter.generated.h"


UCLASS(config=Game)
class AMyFPSProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyFPSProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//生命值
	UPROPERTY(BlueprintReadWrite,ReplicatedUsing=OnRep_Health)
	float Health;
	//能量值
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Replicated)
	float Energy;
	//每秒自动回复的能量值
	UPROPERTY(EditAnywhere)
	float EnergyRegenerationRate;
	//每一跳消耗的能力值
	UPROPERTY(EditAnywhere)
	float ConsumeEnergyOfJump;
	//正常前进速度
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	//加速时前进速度
	UPROPERTY(EditAnywhere)
	float MaxSpeed;
	//加速跑每秒消耗的蓝量
	UPROPERTY(EditAnywhere)
	float ConsumeEnergyOfRun;
	//当前速度
	float Speed;
	//表示玩家身上是否带有弹药
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool HasAmmo;
	
	//碰撞到炮台后进行调用，切换控制器
	//UFUNCTION(Client,Reliable)
	//void OnBeginOverlapGun(class APlayerController* CurPlayerController);


	/*测试RPC使用的函数、成员组件*/
	//碰撞到"TestBox"后调用的RPC_Implementation
	UFUNCTION(Client,Reliable)
	void OnBeginOverlapWithTestBox();
	//粒子,测试RPC调用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireParticle;
	

	//玩家是否死亡
	bool IsDied;

protected:

	/**Health复制更新前调用的函数**/
	UFUNCTION()
	void OnRep_Health();

	/*属性复制*/
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*重新定义跳跃事件，方便对Energy进行处理*/
	void StartJump();

	/*每帧执行，自动回蓝*/
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/*加速跑*/
	void Run();
	/*停止加速跑*/
	void StopRun();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/*玩家Health状态改变时执行*/
	void OnHealthChange();
	/**Health设置、获取相关的函数**/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float Damage);
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurHealth() const { return Health; };

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

