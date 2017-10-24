// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ATank* ControlledTank = Cast<ATank>(GetPawn());
    ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (ControlledTank && PlayerTank)
    {
        FVector PlayerLocation = PlayerTank->GetActorLocation();
        MoveToActor(PlayerTank, AcceptanceRadius);

        // Aim towards player
        ControlledTank->AimAt(PlayerLocation);
        ControlledTank->Fire(); // TODO: limit firing rate
    }
}