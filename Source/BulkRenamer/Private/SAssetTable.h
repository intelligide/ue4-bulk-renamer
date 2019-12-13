// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STableRow.h"
#include "SScrollBox.h"

struct FAssetTableRowData
{
	FAssetTableRowData(const FString& InFilename, const FString& InNewFilename, const FString& InPath):
		Filename(InFilename),
		NewFilename(InNewFilename),
		Path(InPath)
	{
	}

	FString Filename;
	FString NewFilename;
	FString Path;
};

class SAssetTable : public SCompoundWidget
{
public:
	/** IDs for list columns */
	static const FName ColumnID_Filename;
	static const FName ColumnID_NewFilename;
	static const FName ColumnID_AssetPath;
	
	SLATE_BEGIN_ARGS(SAssetTable) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TArray<TSharedPtr<FAssetTableRowData>>& InRows);

	void Refresh();

private:
	TSharedRef<ITableRow> GenerateRow(const TSharedPtr<FAssetTableRowData> Item, const TSharedRef< STableViewBase >& OwnerTable);

	FName SortByColumn;
	EColumnSortMode::Type SortMode;
	void SortColumn(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);
	EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

	TArray<TSharedPtr<FAssetTableRowData>> Rows;

	TSharedPtr<SListView<TSharedPtr<FAssetTableRowData>>> ListView;
};
