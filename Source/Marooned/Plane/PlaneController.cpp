// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/PlaneController.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlaneController::APlaneController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlaneController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlaneController::GenerateLandingPath(USplineComponent* landingPath)
{	
	FVector myLocation = GetActorLocation();
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetFocalLocation();
	FVector vecToPlayer = playerLocation - myLocation;

	float numPoints = 10;
	float steepness = 5.0;

    for (int i = 1; i < numPoints; ++i) {
        float t = static_cast<float>(i) / numPoints;
        float heightDecay = FMath::Exp(-steepness * t);
        FVector point = myLocation + vecToPlayer * t;
        point.Z = myLocation.Z * heightDecay;
        landingPath->AddSplinePoint(point, ESplineCoordinateSpace::World);
    }

	// Add the last point at the player's location directly (since e^-x won't be 0 exactly)
	landingPath->AddSplinePoint(playerLocation, ESplineCoordinateSpace::World);
}

FVector APlaneController::calculateCorrectionVector(
	USplineComponent* landingPath, 
	float deltaTime, 
	FVector currentLocation,
	FVector currentHeading,
	FVector& previousError
)
{
	FVector closestSplineTangent = landingPath->FindDirectionClosestToWorldLocation(currentLocation, ESplineCoordinateSpace::World);

	FVector error = closestSplineTangent - currentHeading;
	FVector derivative = (error - previousError) / deltaTime;
	previousError = error;
	
	return kP * error + kD * derivative;
}