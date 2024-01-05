// Copyright (C) 2023 owoDra

#pragma once

#include "AssetTypeActions_Base.h"

#include "AssetTypeCategories.h"

class UClass;


class FAssetTypeActions_ContextEffectLibrary : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override 
	{
		return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ContextEffectLibrary", "ContextEffectLibrary"); 
	}

	virtual FColor GetTypeColor() const override 
	{
		return FColor(65, 200, 98); 
	}

	virtual uint32 GetCategories() override 
	{ 
		return EAssetTypeCategories::Gameplay; 
	}

	virtual UClass* GetSupportedClass() const override;

};
