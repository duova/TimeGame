// Copyright (C) Varian Daemon 2023. All Rights Reserved.

#pragma once

#include "IPropertyTypeCustomization.h"

class UAC_Inventory;
struct FS_CompatibilitySettings;

class FS_InventoryItem_Customization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	
	// BEGIN IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	bool OnFilterAsset(const FAssetData& AssetData, FS_CompatibilitySettings CompatibilitySettings, UAC_Inventory* Inventory);
	// END IPropertyTypeCustomization interface
};
