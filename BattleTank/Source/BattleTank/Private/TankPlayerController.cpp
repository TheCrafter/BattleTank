// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ATank* tank = GetControlledTank();
    if (tank)
        UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possesing: %s"), *tank->GetName());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
    ATank* ControlledTank = GetControlledTank();
    if (!ControlledTank)
        return;

    FVector HitLocation; // Out parameter
    if (GetSightRayHitLocation(OUT HitLocation))
    {
        ControlledTank->AimAt(HitLocation);
    }
}

// Get world location if linetrace through corsshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    bool Result = false;

    /// Get LookDirection
    FVector LookDirection;
    if (GetLookDirection(LookDirection))
    {
        FVector HitLocation;
        Result = GetLookVectorHitLocation(LookDirection, HitLocation);
    }

    return Result;
}

bool ATankPlayerController::GetLookDirection(FVector& LookDirection) const
{
    /// Find crosshair position
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(OUT ViewportSizeX, OUT ViewportSizeY);
    FVector2D ScreenLocation(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

    /// "De-project" the screen position of the crosshair to a world direction
    FVector WorldLocation;
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OUT WorldLocation, OUT LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    FVector EndLocation = StartLocation + LookDirection * LineTraceRange;
    if (GetWorld()->LineTraceSingleByChannel(
        OUT HitResult,
        StartLocation,
        EndLocation,
        ECollisionChannel::ECC_Visibility))
    {
        HitLocation = HitResult.Location;
        return true;
    }

    return false;
}
