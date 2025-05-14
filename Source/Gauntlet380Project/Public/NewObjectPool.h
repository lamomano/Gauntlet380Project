// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PooledObject.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewObjectPool.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAUNTLET380PROJECT_API UNewObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewObjectPool();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	APooledObject* SpawnPooledObject();

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<class APooledObject> PooledObjectSubclass;


	// this is the starting pool size, max pool size will start to recycle the old things in the pool
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int poolSize = 20;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int maxPoolSize = 100;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float pooledObjectLifespan = 0.0f;

	UFUNCTION()
	void OnPooledObjectDespawn(APooledObject* poolActor);

private:

	UFUNCTION()
	APooledObject* CreateObjectClone(int givenIndex);

protected:

	// use TArrays because they dynamically expand and shrink based on the amount of elements it is currently holding
	// use UPROPERTY() to prevent pooled objects from being destroyed by garbage collector

	UPROPERTY()
	TArray<APooledObject*> objectPool;
	UPROPERTY()
	TArray<int> spawnedPoolIndexes;

	UPROPERTY()
	UWorld* world;
};
