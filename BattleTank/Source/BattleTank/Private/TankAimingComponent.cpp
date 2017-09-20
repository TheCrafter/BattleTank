// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
    Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
    if (Barrel)
    {
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
            FVector AimDirection = OutLaunchVelocity.GetSafeNormal(); 
            MoveBarrelTowards(AimDirection);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s fail to aim"), *GetOwner()->GetName());
        }
    }
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    // Work-out difference between current barrel rotation and AimDirection
    FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
    FRotator AimAsRotator = AimDirection.Rotation();
    FRotator DeltaRotator = AimAsRotator - BarrelRotator;
    UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *DeltaRotator.ToString());

    Barrel->Elevate(5); // TODO remove magic number
}