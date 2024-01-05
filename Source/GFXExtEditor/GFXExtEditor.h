// Copyright (C) 2023 owoDra

#pragma once

#include "Modules/ModuleManager.h"

class IAssetTypeActions;


/**
 *  Modules for the Editor features of the Game Effect Extension plugin
 */
class FGFXExtEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TWeakPtr<IAssetTypeActions> ContextEffectLibraryAssetAction;

};
