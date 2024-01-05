// Copyright (C) 2023 owoDra


#include "ContextEffectComponent.h"

#include "ContextEffectSubsystem.h"
#include "EffectDeveloperSettings.h"

#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
	LoadAndAddContextEffectLibraries();
}


void UContextEffectComponent::LoadAndAddContextEffectLibraries()
{
	if (!ContextEffectLibraries.IsEmpty())
	{
		const auto* World{ GetWorld() };

		if (auto* Subsystem{ World ? World->GetSubsystem<UContextEffectSubsystem>() : nullptr })
		{
			Subsystem->LoadAndAddContextEffectLibraries(GetOwner(), ContextEffectLibraries);
		}
	}
}

void UContextEffectComponent::UnloadAndRemoveContextEffectLibraries()
{
	const auto* World{ GetWorld() };

	if (auto* Subsystem{ World ? World->GetSubsystem<UContextEffectSubsystem>() : nullptr })
	{
		Subsystem->UnloadAndRemoveContextEffectLibraries(GetOwner());
	}
}


void UContextEffectComponent::UpdateEffectContexts(FGameplayTagContainer NewEffectContexts)
{
	EffectContexts.Reset(NewEffectContexts.Num());
	EffectContexts.AppendTags(NewEffectContexts);
}

void UContextEffectComponent::UpdateLibraries(TSet<TSoftObjectPtr<UContextEffectLibrary>> NewContextEffectLibraries)
{
	UnloadAndRemoveContextEffectLibraries();

	ContextEffectLibraries = NewContextEffectLibraries;

	LoadAndAddContextEffectLibraries();
}


void UContextEffectComponent::ContextEffectAction_Implementation(FContextEffectGenericParameter Param)
{
	TArray<UAudioComponent*> AudioComponentsToAdd;
	TArray<UNiagaraComponent*> NiagaraComponentsToAdd;
	FGameplayTagContainer TotalContexts;

	// Aggregate contexts

	TotalContexts.AppendTags(Param.Contexts);
	TotalContexts.AppendTags(EffectContexts);

	// Check if converting Physical Surface Type to Context

	if (bConvertPhysicalSurfaceToContext)
	{
		auto PhysicalMaterial{ Param.HitResult.PhysMaterial };

		if (PhysicalMaterial.IsValid())
		{
			auto PhysicalSurfaceType{ PhysicalMaterial->SurfaceType };

			const auto* DevSettings{ GetDefault<UEffectDeveloperSettings>() };

			// Convert Surface Type to known

			if (const auto* FoundSurfaceContext{ DevSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType) })
			{
				FGameplayTag SurfaceContext{ *FoundSurfaceContext };

				TotalContexts.AddTag(SurfaceContext);
			}
		}
	}

	// Cache Active Audio Components 

	for (auto ActiveAudioComponent : ActiveAudioComponents)
	{
		if (ActiveAudioComponent)
		{
			AudioComponentsToAdd.Add(ActiveAudioComponent.Get());
		}
	}

	// Cache Active Niagara Components 
	
	for (auto ActiveNiagaraComponent : ActiveNiagaraComponents)
	{
		if (ActiveNiagaraComponent)
		{
			NiagaraComponentsToAdd.Add(ActiveNiagaraComponent.Get());
		}
	}

	// Spawn effects

	const auto* World{ GetWorld() };

	if (auto* ContextEffectSubsystem{ World ? World->GetSubsystem<UContextEffectSubsystem>() : nullptr })
	{
		TArray<UAudioComponent*> AudioComponents;
		TArray<UNiagaraComponent*> NiagaraComponents;

		ContextEffectSubsystem->SpawnContextEffect(
			GetOwner(),
			Param.Component.Get(),
			Param.Bone,
			Param.LocationOffset, 
			Param.RotationOffset, 
			Param.EffectTag, 
			TotalContexts,
			AudioComponents, 
			NiagaraComponents, 
			Param.VFXScale, 
			Param.AudioVolume, 
			Param.AudioPitch);

		AudioComponentsToAdd.Append(AudioComponents);
		NiagaraComponentsToAdd.Append(NiagaraComponents);
	}

	// Append Active Audio Components

	ActiveAudioComponents.Empty();
	ActiveAudioComponents.Append(AudioComponentsToAdd);

	// Append Active

	ActiveNiagaraComponents.Empty();
	ActiveNiagaraComponents.Append(NiagaraComponentsToAdd);
}
