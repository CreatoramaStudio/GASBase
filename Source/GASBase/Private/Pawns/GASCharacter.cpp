// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/GASCharacter.h"

#include "LogGASBase.h"
#include "AI/GASAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawns/Components/GASCharacterMovementComponent.h"
#include "Pawns/AbilitySystem/Abilities/GASGameplayAbility.h"
#include "Pawns/Components/GASAbilityComponent.h"
#include "Pawns/Components/GASAttributeComponent.h"

// Sets default values
AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGASCharacterMovementComponent>(CharacterMovementComponentName))
{
	AttributeComponent = CreateDefaultSubobject<UGASAttributeComponent>(FName("AttributeComponent"));
	if (AttributeComponent)
	{
		
	}

	AbilityComponent = CreateDefaultSubobject<UGASAbilityComponent>(FName("AbilityComponent"));
	if (AbilityComponent)
	{
		
	}
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	
	bAlwaysRelevant = true;

	AIControllerClass = AGASAIController::StaticClass();
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilityComponent ? AbilityComponent->GetAbilitySystemComponent() : nullptr;
}

// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilityComponent)
	{
		AbilityComponent->OnPlayerState(this);
	}
}



