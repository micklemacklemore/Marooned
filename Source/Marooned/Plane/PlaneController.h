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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane")
	float kD = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane")
	float kP = 1.0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Plane")
	void GenerateLandingPath(USplineComponent* landingPath);

	UFUNCTION(BlueprintCallable, Category = "Plane")
	FVector calculateCorrectionVector(
		USplineComponent* landingPath, 
		float deltaTime, 
		FVector currentLocation,
		FVector currentHeading,
		UPARAM(ref) FVector& previousError
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
