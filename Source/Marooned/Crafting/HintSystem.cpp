#include "HintSystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Crafting/CraftingLog.h"
#include "Engine/DataTable.h"

UHintSystem::UHintSystem() {
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/Marooned/Assets/UI/CraftingHintTable.CraftingHintTable'"));
    if (DataTableAsset.Succeeded())
    {
        CraftingHintTable = DataTableAsset.Object;
        UE_LOG(LogTemp, Display, TEXT("CraftingHintTable loaded successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load CraftingHintTable"));
        return;
    }
}

void UHintSystem::Initialize(
    const TMap<FString, TSubclassOf<ACraftable>>& craftingNamesToClasses
) {
    UE_LOG(LogTemp, Display, TEXT("Initializing Hint System"));

    CraftingNamesToClasses = craftingNamesToClasses;
    LoadCraftableTreeFromFile();
}

bool UHintSystem::LoadCraftableTreeFromFile() {
    FString ProjectDir = FPaths::ProjectDir();
    FString FilePath = FPaths::ConvertRelativePathToFull(ProjectDir + "Content/Data/CraftingTree.json");

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
        return false;
    }

    return ParseCraftableTreeNode(JsonObject, CraftableTree);
}

bool UHintSystem::ParseCraftableTreeNode(TSharedPtr<FJsonObject> JsonObject, CraftingTreeNode& Node) {

    FString ResourceName;
    if (!JsonObject->TryGetStringField(TEXT("ResourceName"), ResourceName))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse ResourceName field"));
        return false;
    }

    if (!CraftingNamesToClasses.Contains(ResourceName))
    {
        UE_LOG(LogTemp, Error, TEXT("Craftable class not found for ResourceName: %s"), *ResourceName);
        return false;
    }

    Node.Craftable = CraftingNamesToClasses[ResourceName];

    const TArray<TSharedPtr<FJsonValue>>* ChildrenArray;
    if (!JsonObject->TryGetArrayField(TEXT("Children"), ChildrenArray))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse Children field"));
        return false;
    }

    for (const TSharedPtr<FJsonValue>& ChildValue : *ChildrenArray)
    {
        TSharedPtr<FJsonObject> ChildObject = ChildValue->AsObject();
        if (!ChildObject.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse Child object"));
            return false;
        }
    
        CraftingTreeNode ChildNode;
        if (!ParseCraftableTreeNode(ChildObject, ChildNode))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse Child node"));
            return false;
        }
    
        Node.Children.Add(ChildNode);
    }

    return true;
}

TArray<FText> UHintSystem::GetHint() {
    CraftingTreeNode NextCraftable = GetNextCraftableForHint(CraftableTree);

#if !UE_BUILD_SHIPPING
    FString ResourceName = NextCraftable.Craftable->GetDefaultObject<ACraftable>()->GetResourceName();
    UE_LOG(LogTemp, Display, TEXT("Next hint for craftable: %s"), *ResourceName);
#endif

    // This isn't super scalable, but we don't have many hints. And it allows us to:
    // 1. Map a craftable class to a hint, rather than a hardcoded string row name. This means we can change the craftable names and not have to update the data table.
    // 2. Have multiple hints for a single craftable class, if we ever want.
    TArray<FText> Hint;
    for (auto& Row : CraftingHintTable->GetRowMap())
    {
        FHintTableEntry* Entry = (FHintTableEntry*)Row.Value;
        if (Entry->CraftableClass == NextCraftable.Craftable)
        {
            Hint = Entry->Hint;
            break;
        }
    }

    return Hint;
}

/**
 * Do a depth-first search through the CraftableTree to find the first craftable class
 * that isn't in the crafting log map yet, and whose children are all in the crafting log map.
 * 
 * DFS is crucial because it helps the player progress along a logically continuous path in the game.
 */
CraftingTreeNode UHintSystem::GetNextCraftableForHint(CraftingTreeNode currentNode) {

    for (CraftingTreeNode& Child : currentNode.Children)
    {
        if (!UCraftingLog::ContainsCraftingLogEntry(Child.Craftable))
        {
            return GetNextCraftableForHint(Child);
        }
    }

    return currentNode;
}