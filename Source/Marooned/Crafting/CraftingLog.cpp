// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingLog.h"

TMap<TSubclassOf<ACraftable>, FCraftingLogEntry> UCraftingLog::m_log = {};

void UCraftingLog::AddLogEntry(TSubclassOf<ACraftable> firstCraftable, TSubclassOf<ACraftable> secondCraftable, TSubclassOf<ACraftable> resultCraftable)
{
#if !UE_BUILD_SHIPPING
    UE_LOG(LogTemp, Display, TEXT("Adding entry to crafting log: %s + %s = %s"), 
            *firstCraftable->GetDefaultObject<ACraftable>()->GetResourceName(), 
            *secondCraftable->GetDefaultObject<ACraftable>()->GetResourceName(), 
            *resultCraftable->GetDefaultObject<ACraftable>()->GetResourceName()
    );
#endif

    FCraftingLogEntry entry;
    entry.firstCraftable = firstCraftable;
    entry.secondCraftable = secondCraftable;

    m_log.Add(resultCraftable, entry);
}

bool UCraftingLog::ContainsCraftingLogEntry(TSubclassOf<ACraftable> resultCraftable)
{
    return m_log.Contains(resultCraftable);
}