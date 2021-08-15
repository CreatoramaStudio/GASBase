// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/GASPawn.h"

// Sets default values
AGASPawn::AGASPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

