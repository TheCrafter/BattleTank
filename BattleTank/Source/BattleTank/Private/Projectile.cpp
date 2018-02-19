// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
    SetRootComponent(CollisionMesh);
    CollisionMesh->SetNotifyRigidBodyCollision(true);
    CollisionMesh->SetVisibility(false);

    LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
    LaunchBlast->AttachTo(RootComponent);
    
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
    ProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::LaunchProjectile(float Speed)
{
    ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
    ProjectileMovementComponent->Activate();
}