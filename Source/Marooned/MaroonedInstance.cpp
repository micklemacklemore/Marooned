// Fill out your copyright notice in the Description page of Project Settings.


#include "MaroonedInstance.h"
#include "Marooned/Crafting/CraftingMatrix.h"

void UMaroonedInstance::Init()
{
    Super::Init();
    CraftingMatrix::Initialize();
}