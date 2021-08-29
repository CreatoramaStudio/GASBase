// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/GASBlueprintFunctionLibrary.h"

FGameplayTag UGASBlueprintFunctionLibrary::RequestGameplayTag(const FName TagName)
{
	return FGameplayTag::RequestGameplayTag(TagName);
}
