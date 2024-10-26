// Copyright (C) Varian Daemon 2023. All Rights Reserved.

#include "InventoryFrameworkEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "ContentBrowserContextMenu/ATA_AttachmentWidget.h"
#include "ContentBrowserContextMenu/ATA_ContainerWidget.h"
#include "ContentBrowserContextMenu/ATA_ItemTrait.h"
#include "ContentBrowserContextMenu/ATA_ItemComponent.h"
#include "ContentBrowserContextMenu/ATA_ItemComponentTrait.h"
#include "ContentBrowserContextMenu/ATA_ItemAsset.h"
#include "ContentBrowserContextMenu/ATA_ItemActor.h"
#include "ContentBrowserContextMenu/ATA_ItemAssetValidation.h"
#include "ContentBrowserContextMenu/ATA_ItemInstance.h"
#include "ContentBrowserContextMenu/ATA_ItemWidget.h"
#include "ContentBrowserContextMenu/ATA_LootPoolObject.h"
#include "ContentBrowserContextMenu/ATA_TagValueCalculation.h"
#include "ContentBrowserContextMenu/ATA_TileWidget.h"
#include "StructCustomization/FS_CompatibilitySettings_Customization.h"
#include "StructCustomization/FS_ContainerSettings_Customization.h"
#include "StructCustomization/FS_InventoryItem_Customization.h"
#include "StructCustomization/FS_TagValue_Customization.h"
#include "StructCustomization/FS_TileTag_Customization.h"
#include "Core/Data/IFP_CoreData.h"
#include "Interfaces/IPluginManager.h"
#include "Other/ItemAssetActorFactory.h"
#include "Other/ItemAssetDetailsCustomization.h"
#include "Other/ItemAssetThumbnailRenderer.h"
#include "Styling/SlateStyleRegistry.h"
#include "Subsystems/PlacementSubsystem.h"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"

#endif

#define LOCTEXT_NAMESPACE "FInventoryFrameworkPluginModule"
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSetInstance->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

void FInventoryFrameworkEditor::StartupModule()
{
	//--------------------
	// Content browser context menu
	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	InventoryFrameworkCategory = AssetTools.RegisterAdvancedAssetCategory(FName("InventoryFrameworkCategory"), FText::FromString("Inventory Framework"));

	Internal_RegisterTypeActions<FATA_ItemAsset>("Item Asset");
	Internal_RegisterTypeActions<FATA_ItemActor>("Item Actor");
	Internal_RegisterTypeActions<FATA_ItemTrait>("Item Trait");
	Internal_RegisterTypeActions<FATA_ItemComponentTrait>("Item Component Trait");
	Internal_RegisterTypeActions<FATA_ItemComponent>("Item Component");
	Internal_RegisterTypeActions<FATA_TagValueCalculation>("Tag Value Calculator");
	Internal_RegisterTypeActions<FATA_ItemWidget>("Item Widget");
	Internal_RegisterTypeActions<FATA_ContainerWidget>("Container Widget");
	Internal_RegisterTypeActions<FATA_TileWidget>("Tile Widget");
	Internal_RegisterTypeActions<FATA_AttachmentWidget>("Attachment Widget");
	Internal_RegisterTypeActions<FATA_ItemAssetValidation>("Item Asset Validation");
	Internal_RegisterTypeActions<FATA_ItemInstance>("Item Instance");
	Internal_RegisterTypeActions<FATA_LootPoolObject>("Loot Pool");

	//--------------------

	
	//--------------------
	// Struct customization
	
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	
	//Register FS_InventoryItem customization
	PropertyEditorModule.RegisterCustomPropertyTypeLayout( FS_InventoryItem::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FS_InventoryItem_Customization::MakeInstance));

	//Register FS_TagValue customization
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FS_TagValue::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FS_TagValue_Customization::MakeInstance));

	//Register FS_TileTag customization
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FS_TileTag::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FS_TileTag_Customization::MakeInstance));

	//Register FS_ContainerSettings customization
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FS_ContainerSettings::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FS_ContainerSettings_Customization::MakeInstance));

	//Register FS_CompatibilitySettings customization
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FS_CompatibilitySettings::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FS_CompatibilitySettings_Customization::MakeInstance));
	
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	//--------------------


	//--------------------
	// Asset customization

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("DA_CoreItem", FOnGetDetailCustomizationInstance::CreateStatic(&UItemAssetDetailsCustomization::MakeInstance));
	
	//--------------------


	//--------------------
	// Actor factories

	/**V: There's a small *gotcha* that the editor module has in IFP.
	 * Because the config references blueprints, it means that when the editor module is loaded,
	 * it'll load those blueprints. And since Blueprints will load all hard references, they can
	 * load blueprints that reference other plugins that haven't been loaded in yet, breaking them.
	 *
	 * Since the placement subsystem loads before the IFP editor module, we have to manually register
	 * the actor factories with the placement subsystem.
	 * This isn't super important, but might be important to note for some people who might be
	 * editing the editor module and need to change the loading order.
	 *
	 * The true fix would be to remove the blueprint references, then this module can have any
	 * loading phase that you want. But for a marketplace asset where I want everything to be
	 * "auto-magical" for 99% of people, I'm leaving this "bug" in this module on purpose.*/
	UItemAssetActorFactory* ItemAssetActorFactory = NewObject<UItemAssetActorFactory>();
	GEditor->ActorFactories.Add(ItemAssetActorFactory);
	if (UPlacementSubsystem* PlacementSubsystem = GEditor->GetEditorSubsystem<UPlacementSubsystem>())
	{
		PlacementSubsystem->RegisterAssetFactory(ItemAssetActorFactory);
	}

	//--------------------


	//--------------------
	// Thumbnail renderers

	UThumbnailManager::Get().RegisterCustomRenderer(UDA_CoreItem::StaticClass(), UItemAssetThumbnailRenderer::StaticClass());
	
	//--------------------
	
	RegisterStyleSet();
}

void FInventoryFrameworkEditor::ShutdownModule()
{
	//--------------------
	// Content browser context menu

	const FAssetToolsModule* AssetToolsModulePtr = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if(AssetToolsModulePtr)
	{
		IAssetTools& AssetTools = AssetToolsModulePtr->Get();
		for(const auto& CurrentAction : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CurrentAction);
		}
	}
	
	//--------------------
	
	
	//--------------------
	// Struct customization
	
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		// unregister properties when the module is shutdown
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		
		PropertyModule.UnregisterCustomPropertyTypeLayout("FS_InventoryItem");
		
		PropertyModule.UnregisterCustomPropertyTypeLayout("FS_TagValue");
		
		PropertyModule.UnregisterCustomPropertyTypeLayout("FS_TileTag");

		PropertyModule.UnregisterCustomPropertyTypeLayout("FS_ContainerSettings");

		PropertyModule.UnregisterCustomPropertyTypeLayout("FS_CompatibilitySettings");
	
		PropertyModule.NotifyCustomizationModuleChanged();
	}
	
	//--------------------

	FModuleManager::Get().OnModulesChanged().RemoveAll(this);

	// Unregister the style set and reset the pointer
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSetInstance.Get());
	StyleSetInstance.Reset();
}

void FInventoryFrameworkEditor::RegisterStyleSet()
{
	StyleSetInstance = MakeShareable(new FSlateStyleSet("IFPStyleSet"));
	
	StyleSetInstance->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("InventoryFrameworkPlugin"))->GetBaseDir() / TEXT("Resources"));

	const FVector2D Icon16(16.0f, 16.0f);
	StyleSetInstance->Set("ClassIcon.AC_Inventory", new IMAGE_BRUSH(TEXT("InventoryComponent_16x"), Icon16));
	//TODO: Add custom item asset icon
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSetInstance);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryFrameworkEditor, InventoryFrameworkEditor)