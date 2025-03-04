// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingLog.h"

TSet<FCraftingLogEntry> UCraftingLog::m_log = {};

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
    entry.resultCraftable = resultCraftable;

    m_log.Add(entry);
}