// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GASAbilitySystemGlobals.generated.h"

/**
* Child class of UAbilitySystemGlobals.
* Do not try to get a reference to this or call into it during constructors of other UObjects. It will crash in packaged games.
*/
UCLASS()
class GASBASE_API UGASAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	public:
	UGASAbilitySystemGlobals();

	/**
	* Cache commonly used tags here. This has the benefit of one place to set the tag FName in case tag names change and
	* the function call into UGASAbilitySystemGlobals::GASGet() is cheaper than calling FGameplayTag::RequestGameplayTag().
	* Classes can access them by UGASAbilitySystemGlobals::GASGet().DeadTag
	* We're not using this in this sample project (classes are manually caching in their constructors), but it's here as a reference.
	*/

	UPROPERTY()
	FGameplayTag DeadTag;

	UPROPERTY()
	FGameplayTag KnockedDownTag;

	static UGASAbilitySystemGlobals& GASGet()
	{
		return dynamic_cast<UGASAbilitySystemGlobals&>(Get());
	}
	
	virtual void InitGlobalTags() override;
};
