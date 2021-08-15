// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitDelayOneFrame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDelayOneFrameDelegate);

/**
* Like WaitDelay but only delays one frame (tick).
*/
UCLASS()
class GASBASE_API UAbilityTask_WaitDelayOneFrame : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitDelayOneFrameDelegate OnFinish;

	virtual void Activate() override;

	// Wait one frame.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitDelayOneFrame* WaitDelayOneFrame(UGameplayAbility* OwningAbility);

	private:
	void OnDelayFinish();
};
