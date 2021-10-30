// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASCharacter.generated.h"

class UGASAttributeComponent;
class UGASAbilityComponent;

UCLASS()
class GASBASE_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = "Character", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGASAttributeComponent* AttributeComponent;

	UPROPERTY(Category = "Character", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGASAbilityComponent* AbilityComponent;
	
protected:

private:

public:

	// Sets default values for this character's properties
	AGASCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Client only
	virtual void OnRep_PlayerState() override;
private:
	
};
