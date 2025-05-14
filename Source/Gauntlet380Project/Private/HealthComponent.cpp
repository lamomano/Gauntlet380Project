// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// ...
	MAX_HEALTH = 100;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();


	CURRENT_HEALTH = MAX_HEALTH;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	// ...
}

void UHealthComponent::DamageTaken(AActor* damagedActor, float damage, const UDamageType* damageType, AController* instigatedBy, AActor* damageCauser)
{
	float change = FMath::Min(damage, CURRENT_HEALTH);
	CURRENT_HEALTH -= change;
	OnHealthDamaged.Broadcast(CURRENT_HEALTH, MAX_HEALTH, change);
	if (CURRENT_HEALTH == 0.0f)
	{
		OnDeath.Broadcast(damagedActor);
	}
}