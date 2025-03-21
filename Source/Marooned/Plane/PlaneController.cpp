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

void APlaneController::GenerateLandingPath(USplineComponent* landingPath, USplineComponent* flightPath, const FVector& planeLocation)
{	
    landingPath->ClearSplinePoints();
    float inputKey = flightPath->FindInputKeyClosestToWorldLocation(planeLocation);
    int nextPointIndex = FMath::CeilToInt(inputKey);
    FVector nextPoint = flightPath->GetLocationAtSplinePoint(nextPointIndex, ESplineCoordinateSpace::World);
    landingPath->AddSplinePoint(planeLocation, ESplineCoordinateSpace::World);
    landingPath->AddSplinePoint(nextPoint, ESplineCoordinateSpace::World);

	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetFocalLocation();
    APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (playerPawn) { // Only available in-game, not sim mode.
        playerLocation = playerPawn->FindComponentByClass<USkeletalMeshComponent>()->GetComponentLocation();
    }

	FVector vecToPlayer = playerLocation - nextPoint;

	float numPoints = 10;
	float steepness = 4.0;
    float initialRotation = 50.0f; 
    float rotationDecayFactor = 4.0f;
    float cumulativeRotation = initialRotation;

    for (int i = 1; i < numPoints; ++i) {
        float t = static_cast<float>(i) / numPoints;
        float heightDecay = FMath::Exp(-steepness * t);
        float rotationDecay = FMath::Exp(-rotationDecayFactor * t);
        FVector point = nextPoint + vecToPlayer * t;

        // Rotate the point around the Z-axis with playerLocation as the pivot
        FVector relativePoint = point - playerLocation;
        FQuat rotationQuat = FQuat(FVector(0, 0, 1), FMath::DegreesToRadians(cumulativeRotation));
        relativePoint = rotationQuat.RotateVector(relativePoint);
        point = relativePoint + playerLocation;
        
        cumulativeRotation += (initialRotation * rotationDecay);
        point.Z = (nextPoint.Z - playerLocation.Z) * heightDecay + playerLocation.Z;
        landingPath->AddSplinePoint(point, ESplineCoordinateSpace::World);
    }

	// Add the last point at the player's location directly (since e^-x won't be 0 exactly)
	landingPath->AddSplinePoint(playerLocation, ESplineCoordinateSpace::World);
    landingPath->SetTangentAtSplinePoint(landingPath->GetNumberOfSplinePoints() - 1, FVector::ZeroVector, ESplineCoordinateSpace::World);

    // Set the 2nd to last point to be the same Z as the last point so the plane lands flat
    FVector lastPoint = landingPath->GetLocationAtSplinePoint(landingPath->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);
    FVector secondToLastPoint = landingPath->GetLocationAtSplinePoint(landingPath->GetNumberOfSplinePoints() - 2, ESplineCoordinateSpace::World);
    secondToLastPoint.Z = lastPoint.Z;
    landingPath->SetLocationAtSplinePoint(landingPath->GetNumberOfSplinePoints() - 2, secondToLastPoint, ESplineCoordinateSpace::World);
}