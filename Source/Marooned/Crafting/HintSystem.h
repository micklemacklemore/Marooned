// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HintSystem.generated.h"

struct CraftingTreeNode {
	FString ResourceName;
	TArray<CraftingTreeNode> Children;
};

/**
 * 
 */
UCLASS()
class MAROONED_API UHintSystem : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();

private:
	static bool LoadCraftableTreeFromFile();
	static bool ParseCraftableTreeNode(TSharedPtr<FJsonObject> JsonObject, CraftingTreeNode& Node);

	static CraftingTreeNode CraftableTree;
};
