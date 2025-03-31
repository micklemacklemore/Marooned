// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Crafting/Craftable.h"
#include "HintSystem.generated.h"

struct CraftingTreeNode {
	TSubclassOf<ACraftable> Craftable;
	TArray<CraftingTreeNode> Children;
};

USTRUCT(BlueprintType)
struct FHintTableEntry : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACraftable> CraftableClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	TArray<FText> Hint;
};

/**
 * 
 */
UCLASS()
class MAROONED_API UHintSystem : public UObject
{
	GENERATED_BODY()

public:
	UHintSystem() = default;

	void Initialize(
		const TMap<FString, TSubclassOf<ACraftable>>& craftingNamesToClasses
	);

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	TArray<FText> GetHint();

private:
	bool LoadCraftableTreeFromFile();
	bool ParseCraftableTreeNode(TSharedPtr<FJsonObject> JsonObject, CraftingTreeNode& Node);
	CraftingTreeNode GetNextCraftableForHint(CraftingTreeNode Node);

	CraftingTreeNode CraftableTree;

	UPROPERTY()
	TMap<FString, TSubclassOf<ACraftable>> CraftingNamesToClasses;
};
