#pragma once
#include "AssetTypeActions_Base.h"
#include "Core/Widgets/W_AttachmentParent.h"

class FATA_AttachmentWidget : public FAssetTypeActions_Base
{
private:
	const EAssetTypeCategories::Type AssetCategory;
	const FText Name;
	const FColor Color;
	
public:

	explicit FATA_AttachmentWidget(EAssetTypeCategories::Type AssetCategory, const FText& Name, const FColor& Color = FColor::Turquoise)
		: AssetCategory(AssetCategory), Name(Name), Color(Color)
	{
	}

	virtual FText GetName() const override { return Name; }
	virtual FColor GetTypeColor() const override { return Color; }
	virtual UClass* GetSupportedClass() const override { return UW_AttachmentParent::StaticClass(); }
	virtual uint32 GetCategories() override { return AssetCategory; }
	virtual const TArray<FText>& GetSubMenus() const override
	{
		static const TArray<FText> SubMenus
		{
			FText::FromString("Widgets")
		};
		return SubMenus;
	}
};
