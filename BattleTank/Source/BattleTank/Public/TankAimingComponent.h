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

    UFUNCTION(BlueprintCallable)
    float GetRemainingReloadTime() const;

    void AimAt(FVector HitLocation);

    EFiringState GetFiringState() const;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Reloading;

private:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    
    UTankBarrel* Barrel = nullptr;
    UTankTurret* Turret = nullptr;
    FVector AimDirection;
    double LastFireTime = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 10000; // TODO find sensible default

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;

    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeInSeconds = 3;

    void MoveBarrelTowards(FVector AimDirection);

    bool IsBarrelMoving() const;
};
