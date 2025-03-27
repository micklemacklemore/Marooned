// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingSpawner.h"
#include "Crafting/CraftingMatrix.h"
#include "Crafting/CraftingLog.h"

void UCraftingSpawner::Initialize(
    const TMap<FString, TSubclassOf<ACraftable>>* craftingNamesToClasses
)
{
    CraftingNamesToClasses = craftingNamesToClasses;
}

ACraftable* UCraftingSpawner::Craft(
    ACraftable* craftableA, 
    ACraftable* craftableB, 
    const FTransform& transform,
    ECraftingSpawnerBranches& branches,
    bool destroyCraftableA,
    bool destroyCraftableB
)
{
    if (!IsValid(craftableA) || !IsValid(craftableB))
    {
        branches = ECraftingSpawnerBranches::Invalid;
        return nullptr;
    }

    const string& craftResultName = CraftingMatrix::GetCraftingResult(
        TCHAR_TO_UTF8(*(craftableA->GetResourceName())),
        TCHAR_TO_UTF8(*(craftableB->GetResourceName()))
    );

#if !UE_BUILD_SHIPPING
    UE_LOG(LogTemp, Display, TEXT("Crafting %s and %s to get %s"), *craftableA->GetResourceName(), *craftableB->GetResourceName(), *FString(craftResultName.c_str()));
#endif
    
    if (craftResultName == CraftingMatrix::NONE)
    {
        branches = ECraftingSpawnerBranches::Invalid;
        return nullptr;
    }
    
    check(CraftingNamesToClasses->Contains(FString(craftResultName.c_str())));

    // Get the world context
    UWorld* World = craftableA->GetWorld();

    branches = ECraftingSpawnerBranches::Valid;
    TSubclassOf<ACraftable> CraftableClass = (*CraftingNamesToClasses)[FString(craftResultName.c_str())];
    
    // Spawn the crafted instance
    ACraftable* CraftedInstance = World->SpawnActor<ACraftable>(CraftableClass, transform);

    // Destroy the input craftables
    if (destroyCraftableA)
        craftableA->Destroy();

    if (destroyCraftableB)
        craftableB->Destroy();

    // Log the crafting
    UCraftingLog::AddLogEntry(craftableA->GetClass(), craftableB->GetClass(), CraftableClass);
    
    return CraftedInstance;
}