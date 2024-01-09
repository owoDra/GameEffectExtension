// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "ContextEffectLibrary.generated.h"

class UActiveContextEffectLibrary;


/**
 * Data for ContextEffect to be registered in ContextEffectLibrary
 */
USTRUCT()
struct GFXEXT_API FContextEffect
{
	GENERATED_BODY()
public:
	FContextEffect() {}

public:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;

};


/**
 * Data asset class for mapiing effect tags and effects
 */
UCLASS(BlueprintType, Const)
class GFXEXT_API UContextEffectLibrary : public UDataAsset
{
	GENERATED_BODY()

	friend class UActiveContextEffectLibrary;

public:
	UContextEffectLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, Category = "Context Effect", meta = (Categories = "ContextEffect"))
	FGameplayTag EffectTag;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Context Effect")
	TArray<FContextEffect> ContextEffects;

	//
	// Maximum number of newly created AudioComponent or NiagaraComponent
	// 
	// Tips:
	//	Recycle and use previously created instances when the number of instances reaches max
	//
	//UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "Context Effect", meta = (ClampMin = 1.00))
	//int32 MaxInstances{ 1 };

};
