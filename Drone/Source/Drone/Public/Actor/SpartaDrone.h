// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaDrone.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class DRONE_API ASpartaDrone : public APawn
{
	GENERATED_BODY()

public:
	ASpartaDrone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/* Basic */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

/* Control */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float MaxTiltAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float TilitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float TilitRestoreSpeed;

private:
	void UpdateVelocity(float DeltaTime);
	void UpdateGravity(float DeltaTime);
	bool IsGround();

	/* ¼Ó·Â */
	FVector PreviousPosition;
	FVector PreviousVelocity;
	FVector CurrentVelocity;

	float GravityStrength;
	float TraceDistance;
	bool bIsGrounded;

/* Enhanced Input */
private:
	UFUNCTION()
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);
	
	UFUNCTION()
	void MoveUpDown(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Roll(const FInputActionValue& Value);
};
