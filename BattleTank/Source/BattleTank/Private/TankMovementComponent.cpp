// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrack, UTankTrack* RightTrack)
{
    this->LeftTrack = LeftTrack;
    this->RightTrack = RightTrack;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
    if (LeftTrack && RightTrack)
    {
        LeftTrack->SetThrottle(Throw);
        RightTrack->SetThrottle(Throw);
    }
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
    if (LeftTrack && RightTrack)
    {
        LeftTrack->SetThrottle(Throw);
        RightTrack->SetThrottle(-Throw);
    }
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    FVector AIForwardIntention = MoveVelocity.GetSafeNormal();
    float ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);
}