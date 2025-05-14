// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObject*, PoolActor);

UCLASS()
class GAUNTLET380PROJECT_API APooledObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APooledObject();

	FOnPooledObjectDespawn OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable, Category = "Pooled Object")
	void Deactivate();

	void SetActive(bool isActive);
	void SetLifespan(float Lifetime);
	void SetPoolIndex(int givenIndex);

	bool IsActive();
	int GetPoolIndex();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivation();

protected:

	bool active;
	float lifespan = 0.0f;
	int poolIndex;

	FTimerHandle LifespanTimer;
};