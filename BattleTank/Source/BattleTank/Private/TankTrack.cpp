// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
    FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
    UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // Two tracks
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
    // TODO: Clamp actual throttle value so player can't over-drive
    FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
    FVector ForceLocation = GetComponentLocation();
    UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}