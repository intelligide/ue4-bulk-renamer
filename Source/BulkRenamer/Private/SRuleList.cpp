// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SRuleList.h"
#include "SAssetTable.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

void SRuleList::Construct(const FArguments& InArgs)
{
	ModifiedDelegate = InArgs._OnModified;

	ChildSlot
	[
		SAssignNew(RulesBox, SScrollBox)
	];
}

void SRuleList::AddRule(UBulkRenamingRule* Rule)
{
	Rules.Add(Rule);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea, false, nullptr, false, NAME_None);
	TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	DetailsView->SetObject(Rule, true);

	DetailsView->OnFinishedChangingProperties().AddRaw(this, &SRuleList::OnRuleChanged);

	RulesBox->AddSlot()
		.Padding(FMargin(0, 0, 0, 3))
		[
			/*SNew(SExpandableArea)
			.AreaTitle(Factory->GetDisplayName())
			.Padding(FMargin(3))
			.BorderImage(FEditorStyle::Get().GetBrush("DetailsView.CategoryTop"))
			.BodyBorderImage(FEditorStyle::Get().GetBrush("DetailsView.CategoryBottom"))
			.BorderBackgroundColor(FLinearColor(.6, .6, .6, 1.0f))
			.BodyBorderBackgroundColor(FLinearColor(1.0, 1.0, 1.0))
			.AreaTitleFont(FEditorStyle::Get().GetFontStyle("DetailsView.CategoryFontStyle"))
			.BodyContent()
			[
				DetailsView.ToSharedRef()
			]*/
			DetailsView.ToSharedRef()
		];
}

void SRuleList::OnRuleChanged(const FPropertyChangedEvent& InEvent)
{
	if (Rules.Num() > 0)
	{
		ModifiedDelegate.ExecuteIfBound();
	}
}
