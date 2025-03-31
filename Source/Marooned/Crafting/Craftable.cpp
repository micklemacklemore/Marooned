// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/Craftable.h"

// Sets default values
ACraftable::ACraftable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACraftable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACraftable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ACraftable::GetResourceName() const
{
	// This assumes that the class is blueprint generated (ends with "_C")
	FString className = GetClass()->GetName();
	return className.Left(className.Len() - 2);
}