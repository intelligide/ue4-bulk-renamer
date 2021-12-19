// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

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
