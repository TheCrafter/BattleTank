// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    ATank* playerTank = GetPlayerTank();
    if (playerTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("TankAIController found player tank: %s"), *playerTank->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TankAIController can't find player tank"));
    }
}

void ATankAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ATank* ControlledTank = GetControlledTank();
    ATank* PlayerTank = GetPlayerTank();
    if (ControlledTank && PlayerTank)
    {
        FVector PlayerLocation = PlayerTank->GetActorLocation();
        // TODO Move towards player

        // Aim towards player
        ControlledTank->AimAt(PlayerLocation);

        // Fire if ready
    }
}

ATank* ATankAIController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
    auto playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    return Cast<ATank>(playerPawn);
}