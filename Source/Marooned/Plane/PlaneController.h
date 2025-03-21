// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaneController.generated.h"

UCLASS()
class MAROONED_API APlaneController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Plane")
	void GenerateLandingPath(USplineComponent* landingPath, USplineComponent* flightPath, const FVector& planeLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
