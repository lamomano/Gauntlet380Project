// Fill out your copyright notice in the Description page of Project Settings.


#include "NewObjectPool.h"
#include "Kismet/GameplayStatics.h"




APooledObject* UNewObjectPool::CreateObjectClone(int givenIndex)
{
	// make sure you return nullptr so that you don't return something undefined
	if (world == nullptr) return nullptr;


	APooledObject* poolableActor = world->SpawnActor<APooledObject>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

	if (poolableActor != nullptr)
	{
		poolableActor->SetPoolIndex(givenIndex);
		poolableActor->SetActive(false);
		poolableActor->OnPooledObjectDespawn.AddDynamic(this, &UNewObjectPool::OnPooledObjectDespawn);

		objectPool.Add(poolableActor);


		return poolableActor;
	}
	return nullptr;
}


void UNewObjectPool::BeginPlay()
{
	// seems to load the world better when initialized here instead of in the constructor
	Super::BeginPlay();

	world = GetWorld();

	if (PooledObjectSubclass != nullptr && world != nullptr)
	{
		for (int i = 0; i < poolSize; i++)
		{
			CreateObjectClone(i);
		}
	}
	// world or subclass is missing
}

// Sets default values for this component's properties
UNewObjectPool::UNewObjectPool()
{
	/*

	if (PooledObjectSubclass != nullptr)
	{
		world = GetWorld();
		//UWorld* const world = GetWorld();

		UE_LOG(LogTemp, Log, TEXT("world established"));

		if (world != nullptr)
		{
			for (int i = 0; i < poolSize; i++)
			{
				CreateObjectClone(i);
			}
		}
	}
	*/
}




APooledObject* UNewObjectPool::SpawnPooledObject()
{
	for (APooledObject* poolableActor : objectPool)
	{
		if (IsValid(poolableActor) && !poolableActor->IsActive())
		{
			poolableActor->SetLifespan(pooledObjectLifespan);
			poolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			poolableActor->SetActive(true);

			spawnedPoolIndexes.Add(poolableActor->GetPoolIndex());

			return poolableActor;
		}
	}


	// if all actors are currently being used, then check to see if we can instance a new one if the max pool size hasn't been reached

	if (spawnedPoolIndexes.Num() < maxPoolSize)
	{
		// lets make an actor then B)
		return CreateObjectClone(objectPool.Num());
	}
	else if (spawnedPoolIndexes.Num() > 0)
	{
		// only gets to this point if actors haven't exceeded very max size yet
		// recycle time baby

		int pooledObjectIndex = spawnedPoolIndexes[0];
		spawnedPoolIndexes.Remove(pooledObjectIndex);
		APooledObject* poolableActor = objectPool[pooledObjectIndex];


		// IsPendingKill() to avoid reusing dead objects or objects that will get deleted soon
		// in other words this object is about to die, don't touch it
		// 
		// actually dont use IsPendingKill(), it will crash your project
		// IsValid is apparently the way to go since IsPendingKill() is deprecated
		if (IsValid(poolableActor))
		{
			// set it to false first just to make sure the object is still active
			poolableActor->SetActive(false);
			poolableActor->SetLifespan(pooledObjectLifespan);
			poolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			poolableActor->SetActive(true);

			spawnedPoolIndexes.Add(poolableActor->GetPoolIndex());

			return poolableActor;
		}
	}

	
	return nullptr;
}

void UNewObjectPool::OnPooledObjectDespawn(APooledObject* poolActor)
{
	spawnedPoolIndexes.Remove(poolActor->GetPoolIndex());
}
