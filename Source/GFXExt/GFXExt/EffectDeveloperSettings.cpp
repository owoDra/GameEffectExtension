// Copyright (C) 2024 owoDra

#include "EffectDeveloperSettings.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EffectDeveloperSettings)


UEffectDeveloperSettings::UEffectDeveloperSettings()
{
	CategoryName = TEXT("Game XXX Extension");
	SectionName = TEXT("Game Effect Extension");
}


void UEffectDeveloperSettings::ConvertPhysicalSurfaceToContext(EPhysicalSurface SurfaceType, FGameplayTagContainer& OutContexts) const
{
	if (const auto* FoundSurfaceContext{ SurfaceTypeToContextMap.Find(SurfaceType) })
	{
		auto SurfaceContext{ *FoundSurfaceContext };

		OutContexts.AddTag(SurfaceContext);
	}
}

void UEffectDeveloperSettings::ConvertPhysicalMaterialToContext(UPhysicalMaterial* PhysicalMaterial, FGameplayTagContainer& OutContexts) const
{
	auto PhysicalSurfaceType{ PhysicalMaterial ? PhysicalMaterial->SurfaceType : TEnumAsByte<EPhysicalSurface>(EPhysicalSurface::SurfaceType_Default) };

	ConvertPhysicalSurfaceToContext(PhysicalSurfaceType, OutContexts);
}

void UEffectDeveloperSettings::ConvertHitResultToContext(const FHitResult& HitResult, FGameplayTagContainer& OutContexts) const
{
	ConvertPhysicalMaterialToContext(HitResult.PhysMaterial.Get(), OutContexts);
}
