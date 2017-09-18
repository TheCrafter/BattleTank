// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"

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

ATank* ATankAIController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
    auto playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    return Cast<ATank>(playerPawn);
}