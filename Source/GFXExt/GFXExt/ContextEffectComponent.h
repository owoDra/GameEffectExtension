// Copyright (C) 2023 owoDra

#pragma once

#include "Component/GFCActorComponent.h"
#include "ContextEffectInterface.h"

#include "GameplayTagContainer.h"

#include "ContextEffectComponent.generated.h"

class UAudioComponent;
class UNiagaraComponent;
class UContextEffectLibrary;


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

protected:
	//
	// Whether auto-convert Physical Surface from Trace Result to Context or not
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Config")
	bool bConvertPhysicalSurfaceToContext{ true };

public:
	//
	// Current Contexts
	//
	UPROPERTY(EditAnywhere, Category = "Effect Context")
	FGameplayTagContainer EffectContexts;

	//
	// Current Libraries for this Actor
	//
	UPROPERTY(EditAnywhere, Category = "Effect Context")
	TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable)
	void UpdateLibraries(TSet<TSoftObjectPtr<UContextEffectLibrary>> NewContextEffectLibraries);


public:
	virtual void ContextEffectAction_Implementation(FContextEffectGenericParameter Param) override;

};
