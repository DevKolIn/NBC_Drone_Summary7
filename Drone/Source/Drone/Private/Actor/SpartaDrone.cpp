// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SpartaDrone.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/DronePlayerController.h"
#include "InputActionValue.h"

ASpartaDrone::ASpartaDrone()
:NormalSpeed(10.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	/* SkeletalMesh Simulate Physics ��Ȱ��ȭ */
	SkeletalMeshComponent->SetSimulatePhysics(false);

	/* ī�޶� ���� */
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

void ASpartaDrone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpartaDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpartaDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	ADronePlayerController* DroneController = CastChecked<ADronePlayerController>(GetController());

	if (DroneController->MoveForwardAction)
	{
		EnhancedInput->BindAction(DroneController->MoveForwardAction, ETriggerEvent::Triggered, this, &ASpartaDrone::MoveForward);
	}
	if (DroneController->MoveRightAction)
	{
		EnhancedInput->BindAction(DroneController->MoveRightAction, ETriggerEvent::Triggered, this, &ASpartaDrone::MoveRight);
	}
	if (DroneController->LookAction)
	{
		EnhancedInput->BindAction(DroneController->LookAction, ETriggerEvent::Triggered, this, &ASpartaDrone::Look);
	}
}

void ASpartaDrone::MoveForward(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector MoveDirection = CameraComponent->GetForwardVector() * InputValue;
	FVector MoveVector = MoveDirection * NormalSpeed;

	AddActorLocalOffset(MoveVector);
}

void ASpartaDrone::MoveRight(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector MoveDirection = CameraComponent->GetRightVector() * InputValue;
	FVector MoveVector = MoveDirection * NormalSpeed;

	AddActorLocalOffset(MoveDirection);
}

void ASpartaDrone::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	FRotator Rotator(InputValue.Y, InputValue.X, 0.f);
	
	CameraComponent->AddLocalRotation(Rotator);
}

