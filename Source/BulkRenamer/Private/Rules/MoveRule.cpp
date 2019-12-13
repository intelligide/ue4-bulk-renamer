// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MoveRule.h"

#define LOCTEXT_NAMESPACE "BulkRenamer"

FString UMoveRule::Apply(const FString& In) const
{
	FString Copy = In;

	int FromIdx = From;
	if (FromIdx < 0 || FromIdx > Copy.Len())
	{
		return Copy;
	}

	int ToIdx = To;
	if (ToIdx < 0 || ToIdx > Copy.Len())
	{
		return Copy;
	}

	int C = Count;
	if (FromIdx + C > Copy.Len())
	{
		C = Copy.Len() - FromIdx;
	}
	if (C >= Copy.Len())
	{
		return Copy;
	}
	
	FString Part = Copy.Mid(FromIdx, C);
	Copy.InsertAt(ToIdx, Part);
	Copy.RemoveAt(FromIdx, C);

	return Copy;
}

#undef LOCTEXT_NAMESPACE
