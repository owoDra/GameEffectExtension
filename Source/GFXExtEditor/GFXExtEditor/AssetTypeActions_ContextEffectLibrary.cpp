// Copyright (C) 2023 owoDra

#include "AssetTypeActions_ContextEffectLibrary.h"

#include "ContextEffectLibrary.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_ContextEffectLibrary::GetSupportedClass() const
{
	return UContextEffectLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE
