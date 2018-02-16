// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
    Reloading,
    Aiming,
    Locked
};

class AProjectile;
class UTankBarrel;
class UTankTurret;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // Sets default values for this component's properties
	UTankAimingComponent();

    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

    UFUNCTION(BlueprintCallable)
    void Fire();

    void AimAt(FVector HitLocation);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Locked;

private:	
    UTankBarrel* Barrel = nullptr;
    UTankTurret* Turret = nullptr;
    double LastFireTime = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 10000; // TODO find sensible default

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;

    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeInSeconds = 3;

    void MoveBarrelTowards(FVector AimDirection);
};
