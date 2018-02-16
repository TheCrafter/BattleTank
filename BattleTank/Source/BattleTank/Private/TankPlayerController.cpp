// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (ensure(AimingComponent))
        FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!ensure(GetPawn())) { return; }
    AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) { return; }

    FVector HitLocation; // Out parameter
    if (GetSightRayHitLocation(OUT HitLocation))
    {
        AimingComponent->AimAt(HitLocation);
    }
}

// Get world location if linetrace through corsshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    /// Get LookDirection
    FVector LookDirection;
    if (GetLookDirection(LookDirection))
        GetLookVectorHitLocation(LookDirection, OutHitLocation);

    return true;
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
    HitLocation = FVector(0);
    return false;
}
