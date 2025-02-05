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
:MoveSpeed(600.f), LookSpeed(300.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	/* SkeletalMesh Simulate Physics ∫Ò»∞º∫»≠ */
	SkeletalMeshComponent->SetSimulatePhysics(false);

	/* ƒ´∏ﬁ∂Û º≥¡§ */
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SphereComponent);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	/* ÏÜçÎèÑ Í≥ÑÏÇ∞ */
	PreviousPosition = FVector::ZeroVector;
	PreviousVelocity = FVector::ZeroVector;

	/* Ï§ëÎ†• */
	GravityStrength = -98.f;
	TraceDistance = 98.f;
	bIsGrounded = false;
}

void ASpartaDrone::BeginPlay()
{
	Super::BeginPlay();
	
	PreviousPosition = GetActorLocation();
}

void ASpartaDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateVelocity(DeltaTime);
	UpdateGravity(DeltaTime);
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
	if (DroneController->MoveUpDownAction)
	{
		EnhancedInput->BindAction(DroneController->MoveUpDownAction, ETriggerEvent::Triggered, this, &ASpartaDrone::MoveUpDown);
	}
	if (DroneController->LookAction)
	{
		EnhancedInput->BindAction(DroneController->LookAction, ETriggerEvent::Triggered, this, &ASpartaDrone::Look);
	}
	if (DroneController->RollAction)
	{
		EnhancedInput->BindAction(DroneController->RollAction, ETriggerEvent::Triggered, this, &ASpartaDrone::Roll);
	}
}

void ASpartaDrone::MoveForward(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FVector MoveDirection(InputValue, 0.f, 0.f);
	FVector MoveVector = MoveDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalOffset(MoveVector);
}

void ASpartaDrone::MoveRight(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector MoveDirection(0.f, InputValue, 0.f);
	FVector MoveVector = MoveDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalOffset(MoveDirection);
}

void ASpartaDrone::MoveUpDown(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector MoveDirection(0.f, 0.f, InputValue);
	FVector MoveVector = MoveDirection * MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalOffset(MoveDirection);
}

void ASpartaDrone::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	FRotator Rotator(InputValue.Y, InputValue.X, 0.f);
	
	CameraComponent->AddLocalRotation(Rotator);
}

void ASpartaDrone::Roll(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%f"), InputValue));
}

void ASpartaDrone::UpdateVelocity(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	CurrentVelocity = (CurrentPosition - PreviousPosition) / DeltaTime;
	PreviousPosition = CurrentPosition;
	PreviousVelocity = CurrentVelocity;
}

void ASpartaDrone::UpdateGravity(float DeltaTime)
{
	bIsGrounded = IsGround();

	if (bIsGrounded == false)
	{
		FVector MoveDirection(0.f, 0.f, GravityStrength * DeltaTime);
		SetActorLocation(GetActorLocation() + MoveDirection);
	}
}

bool ASpartaDrone::IsGround()
{
	FHitResult HitResult;
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos - FVector(0, 0, TraceDistance);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, TraceParams);

	return HitResult.bBlockingHit == true;
}
