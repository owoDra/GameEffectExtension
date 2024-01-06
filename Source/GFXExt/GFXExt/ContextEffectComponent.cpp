// Copyright (C) 2023 owoDra


#include "ContextEffectComponent.h"

#include "ContextEffectLibrary.h"
#include "ActiveContextEffectLibrary.h"
#include "EffectDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectComponent)


UContextEffectComponent::UContextEffectComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}


void UContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnloadAndRemoveContextEffectLibraries();

	Super::EndPlay(EndPlayReason);
}

void UContextEffectComponent::HandleChangeInitStateToDataInitialized(UGameFrameworkComponentManager* Manager)
{
	if (!ContextEffectLibraries.IsEmpty() && ActiveContextEffectLibraries.IsEmpty())
	{
		LoadAndAddContextEffectLibraries();
	}
}


void UContextEffectComponent::LoadAndAddContextEffectLibraries()
{
	// If the library is already loaded, unload it

	if (!ActiveContextEffectLibraries.IsEmpty())
	{
		UnloadAndRemoveContextEffectLibraries();
	}

	// Load all ContextEffectLibrary

	for (const auto& LibrarySoftObj : ContextEffectLibraries)
	{
		const auto* Library{ LibrarySoftObj.IsValid() ? LibrarySoftObj.Get() : LibrarySoftObj.LoadSynchronous() };

		// If ContextEffectLibrary is successfully loaded, create and initialize ActiveContextEffectLibrary

		if (Library && Library->EffectTag.IsValid())
		{
			auto* NewActiveContextEffectLibrary{ NewObject<UActiveContextEffectLibrary>(this) };
			NewActiveContextEffectLibrary->LoadContextEffectLibrary(Library);

			ActiveContextEffectLibraries.Add(Library->EffectTag, NewActiveContextEffectLibrary);
		}
	}
}

void UContextEffectComponent::UnloadAndRemoveContextEffectLibraries()
{
	for (const auto& KVP : ActiveContextEffectLibraries)
	{
		const auto& ActiveLibrary{ KVP.Value };

		if (ActiveLibrary)
		{
			ActiveLibrary->UnloadContextEffectLibrary();
		}
	}

	ActiveContextEffectLibraries.Empty();
}


void UContextEffectComponent::UpdateExtraContexts(FGameplayTagContainer NewContexts)
{
	ExtraContexts.Reset(NewContexts.Num());
	ExtraContexts.AppendTags(NewContexts);
}

void UContextEffectComponent::UpdateContextEffectLibraries(TSet<TSoftObjectPtr<UContextEffectLibrary>> NewContextEffectLibraries)
{
	ContextEffectLibraries = NewContextEffectLibraries;

	LoadAndAddContextEffectLibraries();
}


void UContextEffectComponent::PlayEffects_Implementation(
	FGameplayTag EffectTag
	, FGameplayTagContainer Contexts
	, USceneComponent* AttachToComponent
	, FName AttachPointName
	, FVector LocationOffset
	, FRotator RotationOffset
	, EAttachLocation::Type LocationType
	, float VolumeMultiplier
	, float PitchMultiplier
	, FVector VFXScale)
{
	auto TotalContexts{ Contexts };
	TotalContexts.AppendTags(ExtraContexts);

	if (auto ActiveContextEffectLibrary{ ActiveContextEffectLibraries.FindRef(EffectTag) })
	{
		ActiveContextEffectLibrary->PlayEffects(
			TotalContexts
			, AttachToComponent
			, AttachPointName
			, LocationOffset
			, RotationOffset
			, LocationType
			, VolumeMultiplier
			, PitchMultiplier
			, VFXScale
		);
	}
}
