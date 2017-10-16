// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
    // auto Time = GetWorld()->GetTimeSeconds();
    UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *GetName(), Throttle);

    // TODO: Clamp actual throttle value so player can't over-drive
}