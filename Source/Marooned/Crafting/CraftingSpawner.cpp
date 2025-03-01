// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingSpawner.h"
#include "Crafting/CraftingMatrix.h"
#include <string>
#include "AssetRegistry/AssetRegistryModule.h"

UCraftingSpawner::UCraftingSpawner()
{
    InitializeCraftingNamesToClasses();
}

void UCraftingSpawner::InitializeCraftingNamesToClasses()
{
    // Get the asset registry module
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    // Force rescan of asset paths
    TArray<FString> PathsToScan;
    PathsToScan.Add(TEXT("/Game/Marooned/Assets/Craftables"));
    AssetRegistry.ScanPathsSynchronous(PathsToScan, true);
    
    // Get all assets in the craftables folder
    // This isn't ideal, because it couples this code to the name of the folder, but GetAssetsByClass doesn't seem to work.
    TArray<FAssetData> AssetDataArray;
    AssetRegistry.GetAssetsByPath(TEXT("/Game/Marooned/Assets/Craftables"), AssetDataArray, true);
    UE_LOG(LogTemp, Display, TEXT("Found %d ACraftable assets"), AssetDataArray.Num());

    for (const FAssetData& AssetData : AssetDataArray)
    {
        UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
        if (Blueprint && Blueprint->GeneratedClass 
            && !Blueprint->GeneratedClass->HasAnyClassFlags(CLASS_Abstract)
            && !Blueprint->GeneratedClass->GetName().StartsWith(TEXT("SKEL_")))
        {
            FString ResourceName = Blueprint->GeneratedClass->GetDefaultObject<ACraftable>()->GetResourceName();
            TSubclassOf<ACraftable> CraftableClass = TSubclassOf<ACraftable>(Blueprint->GeneratedClass);
            CraftingNamesToClasses.Add(ResourceName, CraftableClass);

#if !UE_BUILD_SHIPPING
            UE_LOG(LogTemp, Display, TEXT("Added %s to crafting spawner map with class %s"), *ResourceName, *Blueprint->GeneratedClass->GetName());
#endif
        }
    }
}

ACraftable* UCraftingSpawner::Craft(
    ACraftable* craftableA, 
    ACraftable* craftableB, 
    const FTransform& transform,
    ECraftingSpawnerBranches& branches
)
{
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
    
    check(CraftingNamesToClasses.Contains(FString(craftResultName.c_str())));

    // Get the world context
    UWorld* World = craftableA->GetWorld();

    branches = ECraftingSpawnerBranches::Valid;
    TSubclassOf<ACraftable> CraftableClass = CraftingNamesToClasses[FString(craftResultName.c_str())];
    
    // Spawn the crafted instance
    ACraftable* CraftedInstance = World->SpawnActor<ACraftable>(CraftableClass, transform);

    // Destroy the input craftables
    craftableA->Destroy();
    craftableB->Destroy();
    
    return CraftedInstance;
}