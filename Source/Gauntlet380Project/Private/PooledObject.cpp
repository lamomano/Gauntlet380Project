// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject()
{
 	

}


void APooledObject::SetActive(bool isActive)
{
    active = isActive;
    SetActorHiddenInGame(!isActive);
    GetWorldTimerManager().SetTimer(LifespanTimer, this, &APooledObject::Deactivate, lifespan, false);
}


void APooledObject::SetLifespan(float Lifetime)
{
    lifespan = Lifetime;
}


void APooledObject::SetPoolIndex(int givenIndex)
{
    poolIndex = givenIndex;
}

void APooledObject::Deactivate()
{
    SetActive(false);
    GetWorldTimerManager().ClearAllTimersForObject(this);
    OnPooledObjectDespawn.Broadcast(this);
}

bool APooledObject::IsActive()
{
    return active;
}

int APooledObject::GetPoolIndex()
{
    return poolIndex;
}