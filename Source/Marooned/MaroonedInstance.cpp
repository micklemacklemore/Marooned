// Fill out your copyright notice in the Description page of Project Settings.


#include "MaroonedInstance.h"
#include "Crafting/CraftingMatrix.h"
#include "Crafting/HintSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UMaroonedInstance::Init()
{
    Super::Init();
    InitializeCraftingNamesToClasses();
    CraftingMatrix::Initialize();
    
    HintSystem = NewObject<UHintSystem>(this);
    HintSystem->Initialize(&CraftingNamesToClasses);

    CraftingSpawner = NewObject<UCraftingSpawner>(this);
    CraftingSpawner->Initialize(&CraftingNamesToClasses);
}

void UMaroonedInstance::InitializeCraftingNamesToClasses()
{
    // Get the asset registry module
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    // Force rescan of asset paths. Not sure if this is actually necessary.
    // What IS necessary, however, is to change the asset manager settings such that all craftable assets are always cooked / included in the build.
    // This also requires overriding the GetPrimaryAssetID() function in Craftable.h
    TArray<FString> PathsToScan;
    PathsToScan.Add(TEXT("/Game/Marooned/Assets/Craftables"));
    AssetRegistry.ScanPathsSynchronous(PathsToScan, true);
    
    // Get all assets in the craftables folder
    // This isn't ideal, because it couples this code to the name of the folder, but GetAssetsByClass doesn't seem to work.
    TArray<FAssetData> AssetDataArray;
    AssetRegistry.GetAssetsByPath(TEXT("/Game/Marooned/Assets/Craftables"), AssetDataArray, true);
    UE_LOG(LogTemp, Display, TEXT("Found %d ACraftable assets"), AssetDataArray.Num());

    // While we're here, load all the assets so we don't have runtime lag
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    for (const FAssetData& AssetData : AssetDataArray)
    {
#if !WITH_EDITOR
        UClass* CraftableClass = Cast<UClass>(AssetData.GetAsset());
#else
        // In the editor, we need to get the generated class from the blueprint. In a packaged game, the class is generated already.
        UBlueprint* blueprint = Cast<UBlueprint>(AssetData.GetAsset());
        UClass* CraftableClass = nullptr;
        if (blueprint && !blueprint->GeneratedClass->HasAnyClassFlags(CLASS_Abstract)
            && !blueprint->GeneratedClass->GetName().StartsWith(TEXT("SKEL_"))) {
            CraftableClass = blueprint->GeneratedClass;
        }
#endif

        if (CraftableClass && CraftableClass->IsChildOf(ACraftable::StaticClass()))
        {
            FString ResourceName = CraftableClass->GetDefaultObject<ACraftable>()->GetResourceName();
            TSubclassOf<ACraftable> CraftableSubclass = CraftableClass;
            CraftingNamesToClasses.Add(ResourceName, CraftableSubclass);

#if !UE_BUILD_SHIPPING
            UE_LOG(LogTemp, Display, TEXT("Added %s to crafting name map with class %s"), *ResourceName, *CraftableClass->GetName());
#endif
            // Preload the craftable asset into memory
            FString AssetPath = AssetData.ObjectPath.ToString();
            Streamable.RequestAsyncLoad(FSoftObjectPath(AssetPath), nullptr);
        }
    }
}