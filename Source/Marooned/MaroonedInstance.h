// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Crafting/CraftingSpawner.h"
#include "Crafting/HintSystem.h"
#include "Crafting/Craftable.h"
#include "MaroonedInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAROONED_API UMaroonedInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION(BluePrintCallable, Category = "Crafting")
	UCraftingSpawner* GetCraftingSpawner() const { return CraftingSpawner; }

	UFUNCTION(BluePrintCallable, Category = "Crafting")
	UHintSystem* GetHintSystem() const { return HintSystem; }

private:
	UPROPERTY(BlueprintGetter=GetCraftingSpawner, EditAnywhere, Category = "Crafting")
    UCraftingSpawner* CraftingSpawner;

	UPROPERTY(BlueprintGetter=GetHintSystem, EditAnywhere, Category = "Crafting")
	UHintSystem* HintSystem;

	void InitializeCraftingNamesToClasses();
	TMap<FString, TSubclassOf<ACraftable>> CraftingNamesToClasses = {};
};