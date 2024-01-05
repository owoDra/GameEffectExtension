// Copyright (C) 2023 owoDra

#include "GFXExtEditor.h"

#include "AssetTypeActions_ContextEffectLibrary.h"

#include "IAssetTools.h"
#include "IAssetTypeActions.h"

IMPLEMENT_MODULE(FGFXExtEditorModule, GFXExtEditor)


void FGFXExtEditorModule::StartupModule()
{
	auto& AssetTools{ FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get() };

	auto AssetAction{ MakeShared<FAssetTypeActions_ContextEffectLibrary>() };
	ContextEffectLibraryAssetAction = AssetAction;
	AssetTools.RegisterAssetTypeActions(AssetAction);
}

void FGFXExtEditorModule::ShutdownModule()
{
	auto* AssetToolsModule{ FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools") };
	auto AssetAction{ ContextEffectLibraryAssetAction.Pin() };

	if (AssetToolsModule && AssetAction)
	{
		AssetToolsModule->Get().UnregisterAssetTypeActions(AssetAction.ToSharedRef());
	}
}
