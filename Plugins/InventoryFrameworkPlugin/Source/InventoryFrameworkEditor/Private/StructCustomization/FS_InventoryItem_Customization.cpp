// Copyright (C) Varian Daemon 2023. All Rights Reserved.

#include "StructCustomization/FS_InventoryItem_Customization.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "DS_InventoryProjectSettings.h"
#include "IDetailTreeNode.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "Core/Components/AC_Inventory.h"
#include "Core/Data/IFP_CoreData.h"
#include "Core/Items/DA_CoreItem.h"

#define LOCTEXT_NAMESPACE "InventoryItemCustomization"

class UDS_InventoryProjectSettings;

TSharedRef<IPropertyTypeCustomization> FS_InventoryItem_Customization::MakeInstance()
{
	// Create the instance and returned a SharedRef
	return MakeShareable(new FS_InventoryItem_Customization());
}

void FS_InventoryItem_Customization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			StructPropertyHandle->CreatePropertyValueWidget()
		];
}

void FS_InventoryItem_Customization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);
	const UDS_InventoryProjectSettings* IFPSettings = GetDefault<UDS_InventoryProjectSettings>();

	//Get the inventory component, container index and item index
	TArray<UObject*> OuterObjects;
	StructPropertyHandle->GetOuterObjects(OuterObjects);
	UAC_Inventory* InventoryComponent = nullptr;
	TSharedPtr<IPropertyHandle> ParentPropertyHandle = StructPropertyHandle->GetParentHandle();
	TSharedPtr<IPropertyHandleArray> ItemsArrayHandle;
	int32 ContainerIndex = -1;
	if(ParentPropertyHandle.IsValid())
	{
		ItemsArrayHandle = ParentPropertyHandle->AsArray();
		if(ItemsArrayHandle.IsValid())
		{
			//The parent handle is the item struct. So we get the parent of that,
			//which is the container struct.
			ContainerIndex = ParentPropertyHandle->GetParentHandle()->GetIndexInArray();
		}
	}

	int32 ItemIndex = StructPropertyHandle->GetIndexInArray();
	FS_ContainerSettings Container = FS_ContainerSettings();
	
	for(auto& CurrentOuter : OuterObjects)
	{
		if(!CurrentOuter) { continue; }
		
		if(Cast<UAC_Inventory>(CurrentOuter))
		{
			InventoryComponent = Cast<UAC_Inventory>(CurrentOuter);
			Container = InventoryComponent->ContainerSettings[ContainerIndex];
			break;
		}
	}

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		auto ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex);

		//We don't want certain properties to be visible.
		if(ChildHandle->GetProperty()->GetName() != GET_MEMBER_NAME_CHECKED(FS_InventoryItem, ItemEditorName) &&
			ChildHandle->GetProperty()->GetName() !=  GET_MEMBER_NAME_CHECKED(FS_InventoryItem, CanRotate) && 
			ChildHandle->GetProperty()->GetName() != GET_MEMBER_NAME_CHECKED(FS_InventoryItem, IsStackable) &&
			ChildHandle->GetProperty()->GetName() != GET_MEMBER_NAME_CHECKED(FS_InventoryItem, AllowItemInstance) &&
			ChildHandle->GetProperty()->GetName() != GET_MEMBER_NAME_CHECKED(FS_InventoryItem, ContainerSupportsTileMap))
		{
			bool UseDefaultBuilder = true;
			
			if(!InventoryComponent)
			{
				//No inventory component found, can't customize
				StructBuilder.AddProperty(ChildHandle.ToSharedRef());
				continue;
			}
			
			if(ChildHandle->GetProperty()->GetName() == GET_MEMBER_NAME_CHECKED(FS_InventoryItem, ItemAsset) && IFPSettings->LimitItemSelectionToCompabilityCheck)
			{
				/**BLACK MAGIC TIME!
				 * Since we can't convert IPropertyHandle to a struct reference,
				 * and GetValue does not work with most structs, we have to do a little
				 * bit of cheating. This will get the inventory component, then retrieve
				 * the correct Container from the parents parent handle, since that handle
				 * stores the index in the array, allowing us to find the compatibility
				 * settings to properly filter out assets.*/
				
				//Check if this is an Items array
				if(ItemsArrayHandle.IsValid())
				{
					// Now access the ContainerSettings array within AC_Inventory
					if(InventoryComponent->ContainerSettings.IsValidIndex(ContainerIndex))
					{
						// Get the container at the given index
						FS_CompatibilitySettings CompatibilitySettings = Container.CompatibilitySettings;
										
						//Filter the dropdown using the inventory component and the compatibility settings.
						StructBuilder.AddProperty(ChildHandle.ToSharedRef())
						.CustomWidget()
						.NameContent()
						[
							ChildHandle->CreatePropertyNameWidget()
						]
						.ValueContent()
						.MaxDesiredWidth(500)
						[
							SNew(SObjectPropertyEntryBox)
							.PropertyHandle(ChildHandle)
							.AllowedClass(UDA_CoreItem::StaticClass())
							.ThumbnailPool(StructCustomizationUtils.GetThumbnailPool())
							.OnShouldFilterAsset(FOnShouldFilterAsset::CreateSP(
								this, &FS_InventoryItem_Customization::OnFilterAsset,
								CompatibilitySettings,
								InventoryComponent))
						]
						.ShouldAutoExpand(true);

						//Customization was successful, don't use default
						UseDefaultBuilder = false;
					}
				}
			}

			if(UseDefaultBuilder)
			{
				StructBuilder.AddProperty(ChildHandle.ToSharedRef());
			}
		}
	}
}

bool FS_InventoryItem_Customization::OnFilterAsset(const FAssetData& AssetData, FS_CompatibilitySettings CompatibilitySettings, UAC_Inventory* Inventory)
{
	FS_InventoryItem ItemData;
	ItemData.ItemAsset = Cast<UDA_CoreItem>(AssetData.GetAsset());
	FS_ContainerSettings ContainerSettings;
	ContainerSettings.CompatibilitySettings = CompatibilitySettings;
	
	return !Inventory->CheckCompatibility(ItemData, ContainerSettings);;
}


#undef LOCTEXT_NAMESPACE
