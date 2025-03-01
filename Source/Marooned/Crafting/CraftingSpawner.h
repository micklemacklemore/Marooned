// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Crafting/Craftable.h"
#include "Engine/World.h"
#include "CraftingSpawner.generated.h"

UENUM(BlueprintType)
enum class ECraftingSpawnerBranches : uint8
{
	Valid,
	Invalid
};

/**
 * 
 */
UCLASS()
class MAROONED_API UCraftingSpawner : public UObject
{
	GENERATED_BODY()

public:
	UCraftingSpawner();

	UFUNCTION( BlueprintCallable, Category = "Crafting", Meta = (ExpandEnumAsExecs = "branches"))
	ACraftable* Craft(
		ACraftable* craftableA, 
		ACraftable* craftableB,
		const FTransform& transform,
		ECraftingSpawnerBranches& branches
	);

private:
	TMap<FString, TSubclassOf<ACraftable>> CraftingNamesToClasses = {};
	void InitializeCraftingNamesToClasses();
};
