#pragma once

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	
	// Ability 1
	Ability1		UMETA(DisplayName = "Ability1"),
	
	// Ability 2
	Ability2		UMETA(DisplayName = "Ability2"),
	
	// Ability 3
	Ability3		UMETA(DisplayName = "Ability3"),
	
	// Ability 4
	Ability4		UMETA(DisplayName = "Ability4"),
	
	// Ability 5
	Ability5		UMETA(DisplayName = "Ability5"),
};
