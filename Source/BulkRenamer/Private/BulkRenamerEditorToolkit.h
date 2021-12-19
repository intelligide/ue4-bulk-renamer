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

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SScrollBox.h"
#include "BulkRenamingRule.h"
#include "SAssetTable.h"
#include "SRuleList.h"

class UBulkRenamingRuleFactory;

class FBulkRenamerEditorToolkit : public FAssetEditorToolkit
{
public:
	FBulkRenamerEditorToolkit();

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	virtual FName GetToolkitFName() const override;

	virtual FText GetBaseToolkitName() const override;

	virtual FText GetToolkitName() const override;

	virtual FText GetToolkitToolTipText() const override;

	FLinearColor GetWorldCentricTabColorScale() const override;

	FString GetWorldCentricTabPrefix() const override;

	virtual bool IsPrimaryEditor() const override { return false; };

	static TSharedRef<FBulkRenamerEditorToolkit> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	static TSharedRef<FBulkRenamerEditorToolkit> CreateEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit);

private:
	FReply Run();
	FString ApplyRules(FString str);
	void OnRulesChanged();

	TSharedRef<SWidget> MakeRulesMenu();

	static TSharedPtr<FBulkRenamerEditorToolkit> FindExistingEditor(UObject* Object);

	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<UObject*>& ObjectsToEdit);

	TSharedRef<SDockTab> SpawnTab_AssetTable(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_RuleList(const FSpawnTabArgs& Args);

	static const FName ToolkitFName;
	static const FName ApplicationId;
	static const FName AssetTableId;
	static const FName MethodListTabId;

	TSharedPtr<SRuleList> RuleList;
	void AddRule(UBulkRenamingRuleFactory* Factory);

	TSharedPtr<SAssetTable> AssetTable;
	TArray<TSharedPtr<FAssetTableRowData>> AssetTableRows;
};
