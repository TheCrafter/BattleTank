// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
    // So that first fire is after initial reload
    LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(
    float DeltaTime,
    enum ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction)
{
    if (RoundsLeft <= 0)
    {
        FiringState = EFiringState::OutOfAmmo;
    }
    else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
    {
        FiringState = EFiringState::Reloading;
    }
    else if (IsBarrelMoving())
    {
        FiringState = EFiringState::Aiming;
    }
    else
    {
        FiringState = EFiringState::Locked;
    }
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

void UTankAimingComponent::Fire()
{
    if (ensure(Barrel) && ensure(ProjectileBlueprint) &&
        (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming))
    {
        // Spawn projectile at the socket location of the barrel
        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
            ProjectileBlueprint,
            Barrel->GetSocketLocation(FName("Projectile")),
            Barrel->GetSocketRotation(FName("Projectile"))
        );
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
        RoundsLeft--;
    }
}

float UTankAimingComponent::GetRemainingReloadTime() const
{
    return ReloadTimeInSeconds - (FPlatformTime::Seconds() - LastFireTime);
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
    if (!ensure(Barrel && Turret)) { return; }

    FVector OutLaunchVelocity = FVector(0);
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
        this,
        OutLaunchVelocity,
        StartLocation,
        HitLocation,
        LaunchSpeed,
        false,
        0,
        0,
        ESuggestProjVelocityTraceOption::DoNotTrace
    );

    if (bHaveAimSolution)
    {
        // Turn OutLaunchVelocity to unit vector
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
    }
}

EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}

int UTankAimingComponent::GetRoundsLeft() const
{
    return RoundsLeft;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    if (!ensure(Barrel && Turret)) { return; }

    // Work-out difference between current barrel rotation and AimDirection
    FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
    FRotator AimAsRotator = AimDirection.Rotation();
    FRotator DeltaRotator = AimAsRotator - BarrelRotator;
    Barrel->Elevate(DeltaRotator.Pitch);
    Turret->Rotate(FMath::Abs(DeltaRotator.Yaw) < 180.0f ? DeltaRotator.Yaw : -DeltaRotator.Yaw);
}

bool UTankAimingComponent::IsBarrelMoving() const
{
    return ensure(Barrel) ? !Barrel->GetForwardVector().Equals(AimDirection, 0.1f) : false;
}