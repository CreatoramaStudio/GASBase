// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GASBASE_API AGASPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

protected:

private:

public:
	AGASPlayerController();

protected:

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

private:
};
