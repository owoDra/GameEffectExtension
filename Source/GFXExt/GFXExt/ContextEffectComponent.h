// Copyright (C) 2023 owoDra

#pragma once

#include "Component/GFCActorComponent.h"
#include "ContextEffectInterface.h"

#include "ContextEffectComponent.generated.h"

class UAudioComponent;
class UNiagaraComponent;
class UContextEffectLibrary;
class UActiveContextEffectLibrary;


UCLASS( ClassGroup=(Custom), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta=(BlueprintSpawnableComponent) )
class GFXEXT_API UContextEffectComponent 
	: public UGFCActorComponent
	, public IContextEffectInterface
{
	GENERATED_BODY()
public:	
	UContextEffectComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void HandleChangeInitStateToDataInitialized(UGameFrameworkComponentManager* Manager) override;

	void LoadAndAddContextEffectLibraries();
	void UnloadAndRemoveContextEffectLibraries();


public:
	//
	// Current Libraries for this Actor
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect Context")
	TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries;

	//
	// Extra Contexts
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect Context")
	FGameplayTagContainer ExtraContexts;

protected:
	//
	// Mapping list of ActiveContextEffectLibrary and EffectTag loaded based on current ContextEffectLibrary
	//
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UActiveContextEffectLibrary>> ActiveContextEffectLibraries;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateExtraContexts(FGameplayTagContainer NewContexts);

	UFUNCTION(BlueprintCallable)
	void UpdateContextEffectLibraries(TSet<TSoftObjectPtr<UContextEffectLibrary>> NewContextEffectLibraries);


public:
	void PlayEffects_Implementation(
		FGameplayTag EffectTag
		, FGameplayTagContainer Contexts
		, USceneComponent* AttachToComponent	= nullptr
		, FName AttachPointName					= NAME_None
		, FVector LocationOffset				= FVector(0.0, 0.0, 0.0)
		, FRotator RotationOffset				= FRotator(0.0, 0.0, 0.0)
		, EAttachLocation::Type LocationType	= EAttachLocation::KeepRelativeOffset
		, float VolumeMultiplier				= 1.0f
		, float PitchMultiplier					= 1.0f
		, FVector VFXScale						= FVector(1.0, 1.0, 1.0)
	) override;

};
