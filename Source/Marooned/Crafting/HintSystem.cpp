#include "HintSystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

CraftingTreeNode UHintSystem::CraftableTree;

void UHintSystem::Initialize() {
    UE_LOG(LogTemp, Display, TEXT("Initializing Hint System"));

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
    if (!JsonObject->TryGetStringField(TEXT("ResourceName"), Node.ResourceName))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse ResourceName field"));
        return false;
    }

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