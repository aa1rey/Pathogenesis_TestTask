// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"
#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
	PatrolIndex = Direction = 0;
	bLooped = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

	Path = CreateDefaultSubobject<USplineComponent>("Patrol Path");
	Path->SetupAttachment(SceneRoot);
}

void APatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Path->SetClosedLoop(bLooped);
}

void APatrolPath::IncrementPatrolRouteFlipFlop()
{
	PatrolIndex += Direction;
	if (PatrolIndex == Path->GetNumberOfSplinePoints()) Direction = -1;
	else if (PatrolIndex == 0) Direction = 1;
}

void APatrolPath::IncrementPatrolRouteLooped()
{
	if (PatrolIndex < Path->GetNumberOfSplinePoints()) PatrolIndex++;
	else PatrolIndex = 0;
}

void APatrolPath::IncrementPatrolRoute()
{
	if (bLooped) IncrementPatrolRouteLooped();
	else IncrementPatrolRouteFlipFlop();
}

FVector APatrolPath::GetSplinePointWorldLocation()
{
	return Path->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}
