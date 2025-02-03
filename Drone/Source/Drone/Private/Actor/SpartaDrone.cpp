// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SpartaDrone.h"

ASpartaDrone::ASpartaDrone()
{
	PrimaryActorTick.bCanEverTick = true;

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

}

