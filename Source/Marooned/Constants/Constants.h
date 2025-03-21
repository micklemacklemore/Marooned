// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Constants.generated.h"

/**
 * 
 */
UCLASS()
class MAROONED_API UConstants : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, Category = "Constants")
	static FName GetPauseMenuTag();
};
