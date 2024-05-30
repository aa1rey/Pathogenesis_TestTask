// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class PATHOGENESISTESTTASK_API APatrolPath : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Components") USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Components") class USplineComponent* Path;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bLooped;
	int32 PatrolIndex, Direction;

	virtual void OnConstruction(const FTransform& Transform) override;
	void IncrementPatrolRouteFlipFlop();
	void IncrementPatrolRouteLooped();

public:
	APatrolPath();
	UFUNCTION(BlueprintCallable) void IncrementPatrolRoute();
	UFUNCTION(BlueprintCallable, BlueprintPure) FVector GetSplinePointWorldLocation();
};
