// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Craftable.generated.h"

UCLASS(Blueprintable)
class MAROONED_API ACraftable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftable();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	FString GetResourceName() const;

	// In order for the asset manager to recognize this class as a primary asset, we need to override this function
	// That way, all craftables can be always cooked and included in packaged builds, even though they're not referenced anywhere.
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
