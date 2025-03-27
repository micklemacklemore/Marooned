// Fill out your copyright notice in the Description page of Project Settings.


#include "MaroonedInstance.h"
#include "Crafting/CraftingMatrix.h"
#include "Crafting/HintSystem.h"

void UMaroonedInstance::Init()
{
    Super::Init();
    UHintSystem::Initialize();
    CraftingMatrix::Initialize();
    CraftingSpawner = NewObject<UCraftingSpawner>(this);
}