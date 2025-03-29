// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Craftable.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/UnrealType.h"
#include "CraftingLog.generated.h"

USTRUCT(BlueprintType)
struct FCraftingLogEntry
{
	GENERATED_BODY()

	TSubclassOf<ACraftable> firstCraftable;
	TSubclassOf<ACraftable> secondCraftable;
};

FORCEINLINE uint32 GetTypeHash(const FCraftingLogEntry& entry)
{
    return HashCombine(GetTypeHash(entry.firstCraftable), GetTypeHash(entry.secondCraftable));
}

/**
 * 
 */
UCLASS()
class MAROONED_API UCraftingLog : public UObject
{
	GENERATED_BODY()

public:
	// Callable from blueprints, but shouldn't be necessary. Called during crafting process.
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static void AddLogEntry(TSubclassOf<ACraftable> firstCraftable, TSubclassOf<ACraftable> secondCraftable, TSubclassOf<ACraftable> resultCraftable);

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static TMap<TSubclassOf<ACraftable>, FCraftingLogEntry> GetCraftingLog() { return m_log; }

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	static bool ContainsCraftingLogEntry(TSubclassOf<ACraftable> resultCraftable);
	
private:
	UCraftingLog() {};
	~UCraftingLog() {};

	static TMap<TSubclassOf<ACraftable>, FCraftingLogEntry> m_log;
};
