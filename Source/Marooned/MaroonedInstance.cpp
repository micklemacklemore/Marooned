// Fill out your copyright notice in the Description page of Project Settings.


#include "MaroonedInstance.h"
#include "Crafting/CraftingMatrix.h"

void UMaroonedInstance::Init()
{
    Super::Init();
    CraftingMatrix::Initialize();
    CraftingSpawner = NewObject<UCraftingSpawner>(this);
}