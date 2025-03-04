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
	TSubclassOf<ACraftable> resultCraftable;

	bool operator==(const FCraftingLogEntry& other) const
	{
		// We only care about the first two craftables, since the result is always the same
		return firstCraftable == other.firstCraftable
			&& secondCraftable == other.secondCraftable;
	}
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
	static TArray<FCraftingLogEntry> GetCraftingLog() { return m_log.Array(); }
	
private:
	UCraftingLog() {};
	~UCraftingLog() {};

	static TSet<FCraftingLogEntry> m_log;
};
