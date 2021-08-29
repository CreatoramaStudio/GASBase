// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASBASE_API UGASBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "GameplayTags", meta = (BlueprintThreadSafe))
	static FGameplayTag RequestGameplayTag(const FName TagName);
	
};
