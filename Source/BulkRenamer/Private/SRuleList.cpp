/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Yoann Potinet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
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
