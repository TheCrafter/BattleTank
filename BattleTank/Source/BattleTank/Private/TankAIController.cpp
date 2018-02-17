// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    APawn* ControlledTank = GetPawn();
    APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (ensure(PlayerTank && ControlledTank))
    {
        FVector PlayerLocation = PlayerTank->GetActorLocation();
        MoveToActor(PlayerTank, AcceptanceRadius);

        // Aim towards player
        UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
        AimingComponent->AimAt(PlayerLocation);

        if (AimingComponent->GetFiringState() == EFiringState::Locked)
            AimingComponent->Fire();
    }
}